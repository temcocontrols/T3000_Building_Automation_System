/**
 * T3000 Service Worker
 * Provides offline functionality and intelligent caching
 */

const CACHE_NAME = 'T3000_v1.2.0';
const CACHE_TTL = 7 * 24 * 60 * 60 * 1000; // 7 days in milliseconds

// Define cache strategies for different resource types
const CACHE_STRATEGIES = {
  STATIC: 'static',
  RUNTIME: 'runtime',
  API: 'api',
  IMAGES: 'images',
  FONTS: 'fonts'
};

// Cache configuration
const CACHE_CONFIG = {
  [CACHE_STRATEGIES.STATIC]: {
    maxEntries: 100,
    maxAgeSeconds: 7 * 24 * 60 * 60, // 7 days
    patterns: [
      /\.js$/,
      /\.css$/,
      /\.html$/,
      /\/static\//
    ]
  },
  [CACHE_STRATEGIES.RUNTIME]: {
    maxEntries: 50,
    maxAgeSeconds: 24 * 60 * 60, // 1 day
    patterns: [
      /\/src\//,
      /\/lib\//
    ]
  },
  [CACHE_STRATEGIES.API]: {
    maxEntries: 200,
    maxAgeSeconds: 5 * 60, // 5 minutes
    patterns: [
      /\/api\//,
      /ws:\/\//
    ]
  },
  [CACHE_STRATEGIES.IMAGES]: {
    maxEntries: 100,
    maxAgeSeconds: 30 * 24 * 60 * 60, // 30 days
    patterns: [
      /\.png$/,
      /\.jpg$/,
      /\.jpeg$/,
      /\.svg$/,
      /\.gif$/,
      /\.webp$/
    ]
  },
  [CACHE_STRATEGIES.FONTS]: {
    maxEntries: 20,
    maxAgeSeconds: 365 * 24 * 60 * 60, // 1 year
    patterns: [
      /\.woff$/,
      /\.woff2$/,
      /\.ttf$/,
      /\.otf$/
    ]
  }
};

// Files to cache immediately on install
const CRITICAL_CACHE_FILES = [
  '/',
  '/index.html',
  '/src/main.ts',
  '/src/App.vue',
  '/src/router/index.ts',
  '/manifest.json'
];

// T3000-specific files to cache
const T3000_CACHE_FILES = [
  '/src/lib/T3000/Hvac/Basic/B.Element.ts',
  '/src/lib/T3000/Hvac/Basic/B.Container.ts',
  '/src/lib/T3000/Hvac/Basic/B.Group.ts',
  '/src/lib/T3000/Hvac/Util/T3Util.ts',
  '/src/lib/T3000/Hvac/Util/T3Svg.js',
  '/src/lib/T3000/Security/T3SecurityUtil.ts',
  '/src/lib/T3000/Performance/PerformanceMonitor.ts'
];

/**
 * Service Worker Event Handlers
 */

// Install event - cache critical resources
self.addEventListener('install', (event) => {
  console.log('T3000 Service Worker: Installing...');

  event.waitUntil(
    Promise.all([
      cacheManager.populateCache(CACHE_STRATEGIES.STATIC, CRITICAL_CACHE_FILES),
      cacheManager.populateCache(CACHE_STRATEGIES.RUNTIME, T3000_CACHE_FILES)
    ]).then(() => {
      console.log('T3000 Service Worker: Installation complete');
      self.skipWaiting(); // Force activation
    }).catch(error => {
      console.error('T3000 Service Worker: Installation failed:', error);
    })
  );
});

// Activate event - clean up old caches
self.addEventListener('activate', (event) => {
  console.log('T3000 Service Worker: Activating...');

  event.waitUntil(
    Promise.all([
      cacheManager.cleanupOldCaches(),
      self.clients.claim()
    ]).then(() => {
      console.log('T3000 Service Worker: Activation complete');
    })
  );
});

// Fetch event - handle network requests
self.addEventListener('fetch', (event) => {
  const request = event.request;

  // Skip non-HTTP requests
  if (!request.url.startsWith('http')) {
    return;
  }

  // Skip WebSocket connections
  if (request.url.includes('ws://') || request.url.includes('wss://')) {
    return;
  }

  event.respondWith(
    networkHandler.handleRequest(request)
  );
});

// Message event - handle messages from main thread
self.addEventListener('message', (event) => {
  const { type, data } = event.data;

  switch (type) {
    case 'CACHE_T3000_DATA':
      cacheManager.cacheT3000Data(data);
      break;
    case 'CLEAR_CACHE':
      cacheManager.clearCache(data.strategy);
      break;
    case 'GET_CACHE_STATS':
      cacheManager.getCacheStats().then(stats => {
        event.ports[0].postMessage(stats);
      });
      break;
    case 'FORCE_UPDATE':
      self.skipWaiting();
      break;
    default:
      console.warn('T3000 Service Worker: Unknown message type:', type);
  }
});

// Background sync event
self.addEventListener('sync', (event) => {
  if (event.tag === 't3000-background-sync') {
    event.waitUntil(backgroundSync.performSync());
  }
});

// Push event for notifications
self.addEventListener('push', (event) => {
  if (event.data) {
    const data = event.data.json();
    event.waitUntil(
      notificationManager.showNotification(data)
    );
  }
});

/**
 * Cache Manager
 */
const cacheManager = {
  /**
   * Populate cache with files
   */
  async populateCache(strategy, files) {
    const cache = await caches.open(`${CACHE_NAME}_${strategy}`);
    const validFiles = files.filter(file => file && typeof file === 'string');

    try {
      await cache.addAll(validFiles);
      console.log(`T3000 Service Worker: Cached ${validFiles.length} files for ${strategy}`);
    } catch (error) {
      console.warn(`T3000 Service Worker: Some files failed to cache for ${strategy}:`, error);

      // Try to cache files individually
      const promises = validFiles.map(async (file) => {
        try {
          await cache.add(file);
        } catch (err) {
          console.warn(`T3000 Service Worker: Failed to cache ${file}:`, err);
        }
      });

      await Promise.allSettled(promises);
    }
  },

  /**
   * Get appropriate cache strategy for URL
   */
  getCacheStrategy(url) {
    for (const [strategy, config] of Object.entries(CACHE_CONFIG)) {
      if (config.patterns.some(pattern => pattern.test(url))) {
        return strategy;
      }
    }
    return CACHE_STRATEGIES.RUNTIME; // Default strategy
  },

  /**
   * Cache T3000 specific data
   */
  async cacheT3000Data(data) {
    const cache = await caches.open(`${CACHE_NAME}_${CACHE_STRATEGIES.API}`);
    const response = new Response(JSON.stringify(data), {
      headers: { 'Content-Type': 'application/json' }
    });

    await cache.put(`/t3000/data/${data.id}`, response);
    console.log('T3000 Service Worker: Cached T3000 data:', data.id);
  },

  /**
   * Clean up old caches
   */
  async cleanupOldCaches() {
    const cacheNames = await caches.keys();
    const oldCaches = cacheNames.filter(name =>
      name.startsWith('T3000_') && !name.startsWith(CACHE_NAME)
    );

    await Promise.all(
      oldCaches.map(name => caches.delete(name))
    );

    if (oldCaches.length > 0) {
      console.log(`T3000 Service Worker: Cleaned up ${oldCaches.length} old caches`);
    }
  },

  /**
   * Clear specific cache
   */
  async clearCache(strategy) {
    if (strategy) {
      await caches.delete(`${CACHE_NAME}_${strategy}`);
      console.log(`T3000 Service Worker: Cleared ${strategy} cache`);
    } else {
      const cacheNames = await caches.keys();
      const t3000Caches = cacheNames.filter(name => name.startsWith(CACHE_NAME));

      await Promise.all(
        t3000Caches.map(name => caches.delete(name))
      );

      console.log('T3000 Service Worker: Cleared all caches');
    }
  },

  /**
   * Get cache statistics
   */
  async getCacheStats() {
    const stats = {};

    for (const strategy of Object.keys(CACHE_STRATEGIES)) {
      try {
        const cache = await caches.open(`${CACHE_NAME}_${strategy}`);
        const keys = await cache.keys();
        stats[strategy] = {
          entryCount: keys.length,
          maxEntries: CACHE_CONFIG[strategy]?.maxEntries || 0
        };
      } catch (error) {
        stats[strategy] = { entryCount: 0, maxEntries: 0 };
      }
    }

    return stats;
  }
};

/**
 * Network Handler
 */
const networkHandler = {
  /**
   * Handle network requests with caching strategies
   */
  async handleRequest(request) {
    const strategy = cacheManager.getCacheStrategy(request.url);

    switch (strategy) {
      case CACHE_STRATEGIES.STATIC:
        return this.cacheFirst(request, strategy);
      case CACHE_STRATEGIES.RUNTIME:
        return this.staleWhileRevalidate(request, strategy);
      case CACHE_STRATEGIES.API:
        return this.networkFirst(request, strategy);
      case CACHE_STRATEGIES.IMAGES:
        return this.cacheFirst(request, strategy);
      case CACHE_STRATEGIES.FONTS:
        return this.cacheFirst(request, strategy);
      default:
        return this.networkFirst(request, strategy);
    }
  },

  /**
   * Cache first strategy - check cache first, fallback to network
   */
  async cacheFirst(request, strategy) {
    const cache = await caches.open(`${CACHE_NAME}_${strategy}`);
    const cachedResponse = await cache.match(request);

    if (cachedResponse) {
      return cachedResponse;
    }

    try {
      const networkResponse = await fetch(request);
      if (networkResponse.ok) {
        cache.put(request, networkResponse.clone());
      }
      return networkResponse;
    } catch (error) {
      return this.getOfflineFallback(request);
    }
  },

  /**
   * Network first strategy - try network first, fallback to cache
   */
  async networkFirst(request, strategy) {
    const cache = await caches.open(`${CACHE_NAME}_${strategy}`);

    try {
      const networkResponse = await fetch(request);
      if (networkResponse.ok) {
        cache.put(request, networkResponse.clone());
      }
      return networkResponse;
    } catch (error) {
      const cachedResponse = await cache.match(request);
      if (cachedResponse) {
        return cachedResponse;
      }
      return this.getOfflineFallback(request);
    }
  },

  /**
   * Stale while revalidate strategy - return cache immediately, update in background
   */
  async staleWhileRevalidate(request, strategy) {
    const cache = await caches.open(`${CACHE_NAME}_${strategy}`);
    const cachedResponse = await cache.match(request);

    // Start network request in background
    const networkPromise = fetch(request).then(response => {
      if (response.ok) {
        cache.put(request, response.clone());
      }
      return response;
    }).catch(() => null);

    // Return cached response immediately if available
    if (cachedResponse) {
      return cachedResponse;
    }

    // Wait for network if no cache
    try {
      return await networkPromise;
    } catch (error) {
      return this.getOfflineFallback(request);
    }
  },

  /**
   * Get offline fallback response
   */
  getOfflineFallback(request) {
    if (request.destination === 'document') {
      return caches.match('/offline.html') || this.createOfflineResponse();
    }

    if (request.destination === 'image') {
      return this.createOfflineImageResponse();
    }

    return this.createOfflineResponse();
  },

  /**
   * Create offline response
   */
  createOfflineResponse() {
    return new Response(
      JSON.stringify({
        error: 'Offline',
        message: 'T3000 is currently offline. Please check your connection.',
        timestamp: Date.now()
      }),
      {
        status: 503,
        statusText: 'Service Unavailable',
        headers: { 'Content-Type': 'application/json' }
      }
    );
  },

  /**
   * Create offline image response
   */
  createOfflineImageResponse() {
    // Simple 1x1 transparent PNG
    const transparentPng = 'iVBORw0KGgoAAAANSUhEUgAAAAEAAAABCAYAAAAfFcSJAAAADUlEQVR42mNkYPhfDwAChwGA60e6kgAAAABJRU5ErkJggg==';

    return new Response(
      Uint8Array.from(atob(transparentPng), c => c.charCodeAt(0)),
      {
        status: 200,
        headers: { 'Content-Type': 'image/png' }
      }
    );
  }
};

/**
 * Background Sync Manager
 */
const backgroundSync = {
  /**
   * Perform background synchronization
   */
  async performSync() {
    console.log('T3000 Service Worker: Performing background sync');

    try {
      // Sync T3000 data
      await this.syncT3000Data();

      // Cleanup expired cache entries
      await this.cleanupExpiredEntries();

      console.log('T3000 Service Worker: Background sync completed');
    } catch (error) {
      console.error('T3000 Service Worker: Background sync failed:', error);
    }
  },

  /**
   * Sync T3000 data
   */
  async syncT3000Data() {
    // Implementation for syncing T3000 data
    const cache = await caches.open(`${CACHE_NAME}_${CACHE_STRATEGIES.API}`);
    const requests = await cache.keys();

    for (const request of requests) {
      if (request.url.includes('/t3000/data/')) {
        try {
          await fetch(request);
        } catch (error) {
          console.warn('T3000 Service Worker: Failed to sync:', request.url);
        }
      }
    }
  },

  /**
   * Cleanup expired cache entries
   */
  async cleanupExpiredEntries() {
    for (const [strategy, config] of Object.entries(CACHE_CONFIG)) {
      const cache = await caches.open(`${CACHE_NAME}_${strategy}`);
      const requests = await cache.keys();

      for (const request of requests) {
        const response = await cache.match(request);
        if (response) {
          const dateHeader = response.headers.get('date');
          if (dateHeader) {
            const age = Date.now() - new Date(dateHeader).getTime();
            if (age > config.maxAgeSeconds * 1000) {
              await cache.delete(request);
            }
          }
        }
      }
    }
  }
};

/**
 * Notification Manager
 */
const notificationManager = {
  /**
   * Show notification
   */
  async showNotification(data) {
    const options = {
      body: data.body || 'T3000 notification',
      icon: '/icons/t3000-icon-192.png',
      badge: '/icons/t3000-badge-72.png',
      tag: data.tag || 't3000-notification',
      renotify: true,
      requireInteraction: data.urgent || false,
      actions: data.actions || [],
      data: data.data || {}
    };

    await self.registration.showNotification(
      data.title || 'T3000',
      options
    );
  }
};

console.log('T3000 Service Worker: Initialized successfully');
