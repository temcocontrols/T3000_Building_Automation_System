/**
 * T3000 Calculation Web Worker
 * Handles heavy computations for T3000 data processing in the background
 */

// Import any necessary utilities
// Note: In a real implementation, you might need to import specific calculation functions

/**
 * Message types for worker communication
 */
const MessageTypes = {
  CALCULATE_HVAC_DATA: 'calculateHvacData',
  PROCESS_MODBUS_REGISTERS: 'processModbusRegisters',
  OPTIMIZE_DRAWING_DATA: 'optimizeDrawingData',
  VALIDATE_PROJECT_DATA: 'validateProjectData',
  COMPRESS_LARGE_DATASET: 'compressLargeDataset',
  ANALYZE_PERFORMANCE_DATA: 'analyzePerformanceData'
};

/**
 * Main message handler
 */
self.onmessage = function(event) {
  const { type, data, id } = event.data;

  try {
    switch (type) {
      case MessageTypes.CALCULATE_HVAC_DATA:
        handleHvacCalculation(data, id);
        break;

      case MessageTypes.PROCESS_MODBUS_REGISTERS:
        handleModbusProcessing(data, id);
        break;

      case MessageTypes.OPTIMIZE_DRAWING_DATA:
        handleDrawingOptimization(data, id);
        break;

      case MessageTypes.VALIDATE_PROJECT_DATA:
        handleProjectValidation(data, id);
        break;

      case MessageTypes.COMPRESS_LARGE_DATASET:
        handleDataCompression(data, id);
        break;

      case MessageTypes.ANALYZE_PERFORMANCE_DATA:
        handlePerformanceAnalysis(data, id);
        break;

      default:
        postMessage({
          id,
          error: `Unknown message type: ${type}`
        });
    }
  } catch (error) {
    postMessage({
      id,
      error: error.message,
      stack: error.stack
    });
  }
};

/**
 * Handle HVAC calculation tasks
 */
function handleHvacCalculation(data, id) {
  const { hvacData, calculationType, parameters } = data;

  const startTime = performance.now();

  let result;

  switch (calculationType) {
    case 'airflow':
      result = calculateAirflow(hvacData, parameters);
      break;
    case 'temperature':
      result = calculateTemperature(hvacData, parameters);
      break;
    case 'pressure':
      result = calculatePressure(hvacData, parameters);
      break;
    case 'energy':
      result = calculateEnergyConsumption(hvacData, parameters);
      break;
    default:
      throw new Error(`Unknown HVAC calculation type: ${calculationType}`);
  }

  const duration = performance.now() - startTime;

  postMessage({
    id,
    result,
    metadata: {
      calculationType,
      duration,
      processedItems: Array.isArray(hvacData) ? hvacData.length : 1
    }
  });
}

/**
 * Handle Modbus register processing
 */
function handleModbusProcessing(data, id) {
  const { registers, operation, config } = data;

  const startTime = performance.now();

  let result;

  switch (operation) {
    case 'parse':
      result = parseModbusRegisters(registers, config);
      break;
    case 'validate':
      result = validateModbusRegisters(registers, config);
      break;
    case 'optimize':
      result = optimizeModbusRegisters(registers, config);
      break;
    case 'convert':
      result = convertModbusData(registers, config);
      break;
    default:
      throw new Error(`Unknown Modbus operation: ${operation}`);
  }

  const duration = performance.now() - startTime;

  postMessage({
    id,
    result,
    metadata: {
      operation,
      duration,
      processedRegisters: registers.length
    }
  });
}

/**
 * Handle drawing data optimization
 */
function handleDrawingOptimization(data, id) {
  const { drawingData, optimizationType } = data;

  const startTime = performance.now();

  let result;

  switch (optimizationType) {
    case 'simplify':
      result = simplifyDrawingPaths(drawingData);
      break;
    case 'compress':
      result = compressDrawingData(drawingData);
      break;
    case 'cluster':
      result = clusterDrawingElements(drawingData);
      break;
    default:
      throw new Error(`Unknown optimization type: ${optimizationType}`);
  }

  const duration = performance.now() - startTime;

  postMessage({
    id,
    result,
    metadata: {
      optimizationType,
      duration,
      originalSize: JSON.stringify(drawingData).length,
      optimizedSize: JSON.stringify(result).length
    }
  });
}

/**
 * Handle project data validation
 */
function handleProjectValidation(data, id) {
  const { projectData, validationRules } = data;

  const startTime = performance.now();

  const validationResult = validateProjectData(projectData, validationRules);

  const duration = performance.now() - startTime;

  postMessage({
    id,
    result: validationResult,
    metadata: {
      duration,
      validatedItems: Object.keys(projectData).length
    }
  });
}

/**
 * Handle large dataset compression
 */
function handleDataCompression(data, id) {
  const { dataset, compressionType } = data;

  const startTime = performance.now();

  let result;

  switch (compressionType) {
    case 'lz':
      result = compressWithLZ(dataset);
      break;
    case 'json':
      result = compressJSON(dataset);
      break;
    case 'binary':
      result = compressToBinary(dataset);
      break;
    default:
      throw new Error(`Unknown compression type: ${compressionType}`);
  }

  const duration = performance.now() - startTime;

  postMessage({
    id,
    result,
    metadata: {
      compressionType,
      duration,
      originalSize: JSON.stringify(dataset).length,
      compressedSize: JSON.stringify(result).length,
      compressionRatio: JSON.stringify(result).length / JSON.stringify(dataset).length
    }
  });
}

/**
 * Handle performance data analysis
 */
function handlePerformanceAnalysis(data, id) {
  const { performanceData, analysisType } = data;

  const startTime = performance.now();

  let result;

  switch (analysisType) {
    case 'trends':
      result = analyzePerformanceTrends(performanceData);
      break;
    case 'anomalies':
      result = detectPerformanceAnomalies(performanceData);
      break;
    case 'bottlenecks':
      result = identifyBottlenecks(performanceData);
      break;
    default:
      throw new Error(`Unknown analysis type: ${analysisType}`);
  }

  const duration = performance.now() - startTime;

  postMessage({
    id,
    result,
    metadata: {
      analysisType,
      duration,
      analyzedDataPoints: performanceData.length
    }
  });
}

// HVAC Calculation Functions
function calculateAirflow(hvacData, parameters) {
  // Simplified airflow calculation
  const { pressure, temperature, humidity } = parameters;

  return hvacData.map(item => {
    const adjustedFlow = item.baseFlow * (1 + (temperature - 20) * 0.01) * (1 - humidity * 0.005);
    return {
      ...item,
      calculatedFlow: adjustedFlow,
      efficiency: Math.min(100, adjustedFlow / item.designFlow * 100)
    };
  });
}

function calculateTemperature(hvacData, parameters) {
  // Simplified temperature calculation
  const { ambient, setpoint, efficiency } = parameters;

  return hvacData.map(item => {
    const deltaT = (setpoint - ambient) * efficiency;
    return {
      ...item,
      calculatedTemp: ambient + deltaT,
      energyRequired: Math.abs(deltaT) * item.capacity * 0.001
    };
  });
}

function calculatePressure(hvacData, parameters) {
  // Simplified pressure calculation
  const { staticPressure, velocity } = parameters;

  return hvacData.map(item => {
    const dynamicPressure = 0.5 * 1.225 * Math.pow(velocity, 2); // ρv²/2
    return {
      ...item,
      totalPressure: staticPressure + dynamicPressure,
      pressureDrop: item.resistance * Math.pow(velocity, 2)
    };
  });
}

function calculateEnergyConsumption(hvacData, parameters) {
  // Simplified energy calculation
  const { operatingHours, efficiency, costPerKwh } = parameters;

  return hvacData.map(item => {
    const energyConsumption = item.power * operatingHours * (1 / efficiency);
    return {
      ...item,
      dailyEnergy: energyConsumption,
      dailyCost: energyConsumption * costPerKwh,
      annualCost: energyConsumption * costPerKwh * 365
    };
  });
}

// Modbus Processing Functions
function parseModbusRegisters(registers, config) {
  return registers.map(register => {
    const { address, dataType, scale, offset } = register;

    let parsedValue = register.value;

    // Apply scaling and offset
    if (scale) parsedValue *= scale;
    if (offset) parsedValue += offset;

    // Convert data type
    switch (dataType) {
      case 'float':
        parsedValue = parseFloat(parsedValue);
        break;
      case 'int':
        parsedValue = parseInt(parsedValue);
        break;
      case 'bool':
        parsedValue = Boolean(parsedValue);
        break;
    }

    return {
      ...register,
      parsedValue,
      timestamp: Date.now()
    };
  });
}

function validateModbusRegisters(registers, config) {
  const errors = [];
  const warnings = [];

  registers.forEach((register, index) => {
    // Check address range
    if (register.address < 0 || register.address > 65535) {
      errors.push(`Register ${index}: Invalid address ${register.address}`);
    }

    // Check data type
    if (!['float', 'int', 'bool', 'string'].includes(register.dataType)) {
      errors.push(`Register ${index}: Invalid data type ${register.dataType}`);
    }

    // Check value range
    if (register.minValue !== undefined && register.value < register.minValue) {
      warnings.push(`Register ${index}: Value ${register.value} below minimum ${register.minValue}`);
    }

    if (register.maxValue !== undefined && register.value > register.maxValue) {
      warnings.push(`Register ${index}: Value ${register.value} above maximum ${register.maxValue}`);
    }
  });

  return {
    isValid: errors.length === 0,
    errors,
    warnings,
    totalRegisters: registers.length
  };
}

function optimizeModbusRegisters(registers, config) {
  // Sort by address for sequential reading
  const sortedRegisters = [...registers].sort((a, b) => a.address - b.address);

  // Group consecutive addresses
  const groups = [];
  let currentGroup = [sortedRegisters[0]];

  for (let i = 1; i < sortedRegisters.length; i++) {
    const current = sortedRegisters[i];
    const previous = currentGroup[currentGroup.length - 1];

    if (current.address === previous.address + 1) {
      currentGroup.push(current);
    } else {
      groups.push(currentGroup);
      currentGroup = [current];
    }
  }

  if (currentGroup.length > 0) {
    groups.push(currentGroup);
  }

  return {
    originalCount: registers.length,
    optimizedGroups: groups,
    readOperations: groups.length,
    efficiency: ((registers.length - groups.length) / registers.length * 100).toFixed(2)
  };
}

function convertModbusData(registers, config) {
  const { outputFormat, includeMetadata } = config;

  let converted = registers.map(register => {
    const base = {
      address: register.address,
      value: register.parsedValue || register.value
    };

    if (includeMetadata) {
      base.metadata = {
        dataType: register.dataType,
        timestamp: register.timestamp || Date.now(),
        quality: register.quality || 'good'
      };
    }

    return base;
  });

  if (outputFormat === 'csv') {
    const headers = Object.keys(converted[0]).join(',');
    const rows = converted.map(item => Object.values(item).join(','));
    return [headers, ...rows].join('\n');
  }

  return converted;
}

// Drawing Optimization Functions
function simplifyDrawingPaths(drawingData) {
  // Douglas-Peucker algorithm for path simplification
  return drawingData.map(element => {
    if (element.type === 'path' && element.points) {
      element.points = simplifyPath(element.points, 1.0); // tolerance = 1.0
    }
    return element;
  });
}

function simplifyPath(points, tolerance) {
  if (points.length <= 2) return points;

  // Find the point with maximum distance from line segment
  let maxDistance = 0;
  let maxIndex = 0;

  for (let i = 1; i < points.length - 1; i++) {
    const distance = pointToLineDistance(points[i], points[0], points[points.length - 1]);
    if (distance > maxDistance) {
      maxDistance = distance;
      maxIndex = i;
    }
  }

  if (maxDistance > tolerance) {
    // Recursively simplify
    const left = simplifyPath(points.slice(0, maxIndex + 1), tolerance);
    const right = simplifyPath(points.slice(maxIndex), tolerance);
    return left.slice(0, -1).concat(right);
  } else {
    return [points[0], points[points.length - 1]];
  }
}

function pointToLineDistance(point, lineStart, lineEnd) {
  const A = point.x - lineStart.x;
  const B = point.y - lineStart.y;
  const C = lineEnd.x - lineStart.x;
  const D = lineEnd.y - lineStart.y;

  const dot = A * C + B * D;
  const lenSq = C * C + D * D;

  if (lenSq === 0) return Math.sqrt(A * A + B * B);

  const param = dot / lenSq;

  let xx, yy;

  if (param < 0) {
    xx = lineStart.x;
    yy = lineStart.y;
  } else if (param > 1) {
    xx = lineEnd.x;
    yy = lineEnd.y;
  } else {
    xx = lineStart.x + param * C;
    yy = lineStart.y + param * D;
  }

  const dx = point.x - xx;
  const dy = point.y - yy;

  return Math.sqrt(dx * dx + dy * dy);
}

function compressDrawingData(drawingData) {
  // Remove redundant data and compress coordinates
  return drawingData.map(element => {
    const compressed = { ...element };

    // Round coordinates to reduce precision
    if (compressed.x) compressed.x = Math.round(compressed.x * 100) / 100;
    if (compressed.y) compressed.y = Math.round(compressed.y * 100) / 100;
    if (compressed.width) compressed.width = Math.round(compressed.width * 100) / 100;
    if (compressed.height) compressed.height = Math.round(compressed.height * 100) / 100;

    // Remove default values
    if (compressed.opacity === 1) delete compressed.opacity;
    if (compressed.rotation === 0) delete compressed.rotation;
    if (compressed.scaleX === 1) delete compressed.scaleX;
    if (compressed.scaleY === 1) delete compressed.scaleY;

    return compressed;
  });
}

function clusterDrawingElements(drawingData) {
  // Group nearby elements for optimized rendering
  const clusters = [];
  const processed = new Set();

  drawingData.forEach((element, index) => {
    if (processed.has(index)) return;

    const cluster = [element];
    processed.add(index);

    // Find nearby elements
    for (let i = index + 1; i < drawingData.length; i++) {
      if (processed.has(i)) continue;

      const other = drawingData[i];
      const distance = Math.sqrt(
        Math.pow(element.x - other.x, 2) + Math.pow(element.y - other.y, 2)
      );

      if (distance < 100) { // Cluster threshold
        cluster.push(other);
        processed.add(i);
      }
    }

    clusters.push(cluster);
  });

  return clusters;
}

// Project Validation Functions
function validateProjectData(projectData, validationRules) {
  const results = {
    isValid: true,
    errors: [],
    warnings: [],
    validatedFields: 0
  };

  Object.keys(validationRules).forEach(field => {
    const rule = validationRules[field];
    const value = projectData[field];

    results.validatedFields++;

    // Required field check
    if (rule.required && (value === undefined || value === null)) {
      results.errors.push(`Field '${field}' is required`);
      results.isValid = false;
      return;
    }

    if (value === undefined || value === null) return;

    // Type check
    if (rule.type && typeof value !== rule.type) {
      results.errors.push(`Field '${field}' must be of type ${rule.type}`);
      results.isValid = false;
    }

    // Range check
    if (rule.min !== undefined && value < rule.min) {
      results.errors.push(`Field '${field}' must be >= ${rule.min}`);
      results.isValid = false;
    }

    if (rule.max !== undefined && value > rule.max) {
      results.errors.push(`Field '${field}' must be <= ${rule.max}`);
      results.isValid = false;
    }

    // Pattern check
    if (rule.pattern && !rule.pattern.test(value)) {
      results.errors.push(`Field '${field}' does not match required pattern`);
      results.isValid = false;
    }

    // Custom validation
    if (rule.validate && !rule.validate(value)) {
      results.warnings.push(`Field '${field}' failed custom validation`);
    }
  });

  return results;
}

// Compression Functions
function compressWithLZ(data) {
  // Simplified LZ-style compression
  const str = JSON.stringify(data);
  const dict = new Map();
  let dictSize = 256;
  let result = [];
  let w = "";

  for (let i = 0; i < str.length; i++) {
    const c = str.charAt(i);
    const wc = w + c;

    if (dict.has(wc)) {
      w = wc;
    } else {
      result.push(dict.get(w) || w.charCodeAt(0));
      dict.set(wc, dictSize++);
      w = c;
    }
  }

  if (w) {
    result.push(dict.get(w) || w.charCodeAt(0));
  }

  return result;
}

function compressJSON(data) {
  // Remove whitespace and optimize JSON structure
  const optimized = JSON.stringify(data, null, 0);

  // Replace common patterns
  return optimized
    .replace(/":"/g, '":"')
    .replace(/","/g, '","')
    .replace(/":true/g, '":!0')
    .replace(/":false/g, '":!1')
    .replace(/":null/g, '":0');
}

function compressToBinary(data) {
  // Convert to binary representation
  const str = JSON.stringify(data);
  const binary = [];

  for (let i = 0; i < str.length; i++) {
    binary.push(str.charCodeAt(i).toString(2).padStart(8, '0'));
  }

  return binary.join('');
}

// Performance Analysis Functions
function analyzePerformanceTrends(performanceData) {
  const metrics = ['memory', 'cpu', 'renderTime', 'loadTime'];
  const trends = {};

  metrics.forEach(metric => {
    const values = performanceData.map(d => d[metric]).filter(v => v !== undefined);

    if (values.length > 1) {
      const average = values.reduce((a, b) => a + b, 0) / values.length;
      const slope = calculateSlope(values);

      trends[metric] = {
        average,
        trend: slope > 0.1 ? 'increasing' : slope < -0.1 ? 'decreasing' : 'stable',
        slope,
        min: Math.min(...values),
        max: Math.max(...values)
      };
    }
  });

  return trends;
}

function detectPerformanceAnomalies(performanceData) {
  const anomalies = [];
  const threshold = 2; // Standard deviations

  ['memory', 'cpu', 'renderTime', 'loadTime'].forEach(metric => {
    const values = performanceData.map(d => d[metric]).filter(v => v !== undefined);

    if (values.length > 10) {
      const mean = values.reduce((a, b) => a + b, 0) / values.length;
      const stdDev = Math.sqrt(values.reduce((sq, n) => sq + Math.pow(n - mean, 2), 0) / values.length);

      values.forEach((value, index) => {
        const zscore = Math.abs((value - mean) / stdDev);
        if (zscore > threshold) {
          anomalies.push({
            metric,
            index,
            value,
            zscore,
            timestamp: performanceData[index].timestamp
          });
        }
      });
    }
  });

  return anomalies;
}

function identifyBottlenecks(performanceData) {
  const bottlenecks = [];

  // Analyze render times
  const renderTimes = performanceData.map(d => d.renderTime).filter(t => t !== undefined);
  if (renderTimes.length > 0) {
    const avgRenderTime = renderTimes.reduce((a, b) => a + b, 0) / renderTimes.length;
    if (avgRenderTime > 16.67) { // 60 FPS threshold
      bottlenecks.push({
        type: 'render',
        severity: avgRenderTime > 33.33 ? 'high' : 'medium',
        average: avgRenderTime,
        recommendation: 'Consider optimizing rendering pipeline'
      });
    }
  }

  // Analyze memory usage
  const memoryUsage = performanceData.map(d => d.memory).filter(m => m !== undefined);
  if (memoryUsage.length > 0) {
    const avgMemory = memoryUsage.reduce((a, b) => a + b, 0) / memoryUsage.length;
    if (avgMemory > 100 * 1024 * 1024) { // 100MB threshold
      bottlenecks.push({
        type: 'memory',
        severity: avgMemory > 500 * 1024 * 1024 ? 'high' : 'medium',
        average: avgMemory,
        recommendation: 'Review memory usage and implement cleanup'
      });
    }
  }

  return bottlenecks;
}

function calculateSlope(values) {
  const n = values.length;
  const xMean = (n - 1) / 2;
  const yMean = values.reduce((a, b) => a + b, 0) / n;

  let numerator = 0;
  let denominator = 0;

  for (let i = 0; i < n; i++) {
    numerator += (i - xMean) * (values[i] - yMean);
    denominator += Math.pow(i - xMean, 2);
  }

  return denominator === 0 ? 0 : numerator / denominator;
}

// Error handling
self.onerror = function(error) {
  console.error('[T3000 Worker] Error:', error);
  postMessage({
    error: error.message,
    filename: error.filename,
    lineno: error.lineno
  });
};

// Cleanup on termination
self.onclose = function() {
  console.log('[T3000 Worker] Worker terminated');
};
