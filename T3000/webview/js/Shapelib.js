MD.Shapelib = function(){
  var current_d, cur_shape_id;
  var canv = svgCanvas;
  var cur_shape;
  var start_x, start_y;
  var svgroot = canv.getRootElem();
  var lastBBox = {};

  $(document).on("mousedown", function(e){
    if (!e.target.closest("#tools_shapelib"))
      $("#tools_shapelib").hide();
  })
  
  // This populates the category list
  var categories = {
    basic: 'Basic',
    object: 'Objects',
    symbol: 'Symbols',
    arrow: 'Arrows',
    flowchart: 'Flowchart',
    temco: 'T3000',
    ui: 'User Interface',
    social: 'Social Web'
  };
  
  var library = {
    'basic': {
      data: {
        "star_points_5": "m1,116.58409l113.82668,0l35.17332,-108.13487l35.17334,108.13487l113.82666,0l-92.08755,66.83026l35.17514,108.13487l-92.08759,-66.83208l-92.08757,66.83208l35.17515,-108.13487l-92.08758,-66.83026z",
        'donut': 'm1,150l0,0c0,-82.29042 66.70958,-149 149,-149l0,0c39.51724,0 77.41599,15.69816 105.35889,43.64108c27.94293,27.94293 43.64111,65.84165 43.64111,105.35892l0,0c0,82.29041 -66.70958,149 -149,149l0,0c-82.29041,0 -149,-66.70959 -149,-149zm74.5,0l0,0c0,41.1452 33.35481,74.5 74.5,74.5c41.14522,0 74.5,-33.3548 74.5,-74.5c0,-41.1452 -33.3548,-74.5 -74.5,-74.5l0,0c-41.14519,0 -74.5,33.35481 -74.5,74.5z',
        "triangle": "m1,280.375l149,-260.75l149,260.75z",
        "right_triangle": "m1,299l0,-298l298,298z",
        "diamond": "m1,150l149,-149l149,149l-149,149l-149,-149z",
        "pentagon": "m1.00035,116.97758l148.99963,-108.4053l148.99998,108.4053l-56.91267,175.4042l-184.1741,0l-56.91284,-175.4042z",
        "hexagon": "m1,149.99944l63.85715,-127.71428l170.28572,0l63.85713,127.71428l-63.85713,127.71428l-170.28572,0l-63.85715,-127.71428z",
        "septagon1": "m0.99917,191.06511l29.51249,-127.7108l119.48833,-56.83673l119.48836,56.83673l29.51303,127.7108l-82.69087,102.41679l-132.62103,0l-82.69031,-102.41679z",
        "heptagon": "m1,88.28171l87.28172,-87.28171l123.43653,0l87.28172,87.28171l0,123.43654l-87.28172,87.28172l-123.43653,0l-87.28172,-87.28172l0,-123.43654z",
        "decagon": "m1,150.00093l28.45646,-88.40318l74.49956,-54.63682l92.08794,0l74.50002,54.63682l28.45599,88.40318l-28.45599,88.40318l-74.50002,54.63681l-92.08794,0l-74.49956,-54.63681l-28.45646,-88.40318z",
        "dodecagon": "m1,110.07421l39.92579,-69.14842l69.14842,-39.92579l79.85159,0l69.14842,39.92579l39.92578,69.14842l0,79.85159l-39.92578,69.14842l-69.14842,39.92578l-79.85159,0l-69.14842,-39.92578l-39.92579,-69.14842l0,-79.85159z",
        "trapezoid": "m1,299l55.875,-298l186.25001,0l55.87498,298z",
        "dialog_balloon_1": "m0.99786,35.96579l0,0c0,-19.31077 15.28761,-34.96524 34.14583,-34.96524l15.52084,0l0,0l74.50001,0l139.68748,0c9.05606,0 17.74118,3.68382 24.14478,10.24108c6.40356,6.55726 10.00107,15.45081 10.00107,24.72416l0,87.41311l0,0l0,52.44785l0,0c0,19.31078 -15.2876,34.96524 -34.14584,34.96524l-139.68748,0l-97.32507,88.90848l22.82506,-88.90848l-15.52084,0c-18.85822,0 -34.14583,-15.65446 -34.14583,-34.96524l0,0l0,-52.44785l0,0z",
        'heart': 'm150,73c61,-175 300,0 0,225c-300,-225 -61,-400 0,-225z',
        "cylinder": "m299.0007,83.77844c0,18.28676 -66.70958,33.11111 -149.00002,33.11111m149.00002,-33.11111l0,0c0,18.28676 -66.70958,33.11111 -149.00002,33.11111c-82.29041,0 -148.99997,-14.82432 -148.99997,-33.11111m0,0l0,0c0,-18.28674 66.70956,-33.1111 148.99997,-33.1111c82.29044,0 149.00002,14.82436 149.00002,33.1111l0,132.44449c0,18.28674 -66.70958,33.11105 -149.00002,33.11105c-82.29041,0 -148.99997,-14.82431 -148.99997,-33.11105z",
        "arrow_up": "m1.49805,149.64304l148.50121,-148.00241l148.50121,148.00241l-74.25061,0l0,148.71457l-148.5012,0l0,-148.71457z",
        "arrow_u_turn": "m1.00059,299.00055l0,-167.62497l0,0c0,-72.00411 58.37087,-130.37499 130.375,-130.37499l0,0l0,0c34.57759,0 67.73898,13.7359 92.18906,38.18595c24.45006,24.45005 38.18593,57.61144 38.18593,92.18904l0,18.625l37.24997,0l-74.49995,74.50002l-74.50002,-74.50002l37.25,0l0,-18.625c0,-30.8589 -25.0161,-55.87498 -55.87498,-55.87498l0,0l0,0c-30.85892,0 -55.875,25.01608 -55.875,55.87498l0,167.62497z",
        "arrow_left_up": "m0.99865,224.5l74.50004,-74.5l0,37.25l111.74991,0l0,-111.75l-37.25,0l74.5,-74.5l74.5,74.5l-37.25,0l0,186.25l-186.24989,0l0,37.25l-74.50005,-74.5z",
        "plaque": "m-0.00197,49.94376l0,0c27.5829,0 49.94327,-22.36036 49.94327,-49.94327l199.76709,0l0,0c0,27.5829 22.36037,49.94327 49.94325,49.94327l0,199.7671l0,0c-27.58289,0 -49.94325,22.36034 -49.94325,49.94325l-199.76709,0c0,-27.58292 -22.36037,-49.94325 -49.94327,-49.94325z",
        "page": "m249.3298,298.99744l9.9335,-39.73413l39.73413,-9.93355l-49.66763,49.66768l-248.33237,0l0,-298.00001l298.00001,0l0,248.33234",
        "cross": "m0.99844,99.71339l98.71494,0l0,-98.71495l101.26279,0l0,98.71495l98.71495,0l0,101.2628l-98.71495,0l0,98.71494l-101.26279,0l0,-98.71494l-98.71494,0z",
        "divide": "m150,0.99785l0,0c25.17819,0 45.58916,20.41097 45.58916,45.58916c0,25.17821 -20.41096,45.58916 -45.58916,45.58916c-25.17822,0 -45.58916,-20.41093 -45.58916,-45.58916c0,-25.1782 20.41093,-45.58916 45.58916,-45.58916zm0,296.25203c-25.17822,0 -45.58916,-20.41095 -45.58916,-45.58917c0,-25.17819 20.41093,-45.58916 45.58916,-45.58916c25.17819,0 45.58916,20.41096 45.58916,45.58916c0,25.17822 -20.41096,45.58917 -45.58916,45.58917zm-134.06754,-193.71518l268.13507,0l0,91.17833l-268.13507,0z",
        "minus": "m0.99887,102.39503l297.49445,0l0,95.2112l-297.49445,0z",
        "times": "m1.00089,73.36786l72.36697,-72.36697l76.87431,76.87368l76.87431,-76.87368l72.36765,72.36697l-76.87433,76.87431l76.87433,76.87431l-72.36765,72.36765l-76.87431,-76.87433l-76.87431,76.87433l-72.36697,-72.36765l76.87368,-76.87431l-76.87368,-76.87431z"
      },
      buttons: []
    },
    'temco': {
      data: {        
          "fan":"M425.789,284.442c2.684-12.293,0.081-25.245-7.141-35.534c-7.204-10.266-18.466-17.099-30.898-18.749  l-95.127-12.62c-0.01-0.017-0.02-0.034-0.029-0.051l36.634-88.694c4.787-11.591,4.5-24.76-0.787-36.132  c-5.3-11.398-15.216-20.129-27.204-23.951C283.108,62.931,264.188,60,245,60c-19.188,0-38.109,2.931-56.237,8.711  c-11.988,3.822-21.904,12.553-27.204,23.952c-5.288,11.371-5.574,24.541-0.787,36.132l36.634,88.692  c-0.01,0.018-0.02,0.035-0.03,0.052l-95.127,12.62c-12.431,1.65-23.693,8.483-30.897,18.748  c-7.222,10.289-9.825,23.241-7.141,35.535c4.059,18.591,10.981,36.442,20.575,53.058c9.595,16.619,21.594,31.54,35.662,44.347  c8.327,7.58,19.225,11.759,30.397,11.758c1.313,0,2.631-0.058,3.948-0.174c12.492-1.107,24.04-7.444,31.684-17.385l58.474-76.047  c0.017,0,0.033,0.001,0.05,0.001c0.017,0,0.033-0.001,0.05-0.001l58.473,76.047c7.645,9.941,19.193,16.277,31.685,17.385  c1.317,0.116,2.633,0.174,3.946,0.174c11.172,0,22.072-4.178,30.397-11.758c14.07-12.809,26.069-27.729,35.663-44.347  C414.81,320.882,421.731,303.03,425.789,284.442z M270,245c0,12.21-8.801,22.397-20.391,24.567c-0.018,0.003-0.035,0.007-0.052,0.01  c-1.479,0.273-3,0.424-4.556,0.424c-1.559,0-3.083-0.15-4.564-0.425c-0.014-0.002-0.027-0.005-0.041-0.007  C228.803,267.399,220,257.211,220,245c0-13.785,11.215-25,25-25S270,231.215,270,245z M188.763,105.311  c1.799-3.87,5.036-6.717,9.114-8.018C213.054,92.454,228.908,90,245,90c16.091,0,31.945,2.454,47.123,7.293  c4.077,1.301,7.314,4.147,9.114,8.018c1.786,3.843,1.879,8.115,0.262,12.031l-32.328,78.268c-7.3-3.587-15.503-5.61-24.171-5.61  c-8.668,0-16.871,2.023-24.171,5.61L188.5,117.342C186.883,113.426,186.976,109.153,188.763,105.311z M162.694,357.759  c-2.583,3.359-6.329,5.415-10.549,5.788c-4.251,0.375-8.336-1.003-11.501-3.885c-11.779-10.722-21.831-23.226-29.877-37.162  c-8.045-13.934-13.847-28.892-17.246-44.456c-0.913-4.182-0.065-8.409,2.387-11.902c2.435-3.469,6.088-5.686,10.288-6.242  l83.948-11.137c1.183,17.413,10.493,32.631,24.173,41.861L162.694,357.759z M396.479,278.045  c-3.396,15.563-9.198,30.52-17.244,44.455s-18.098,26.438-29.878,37.163c-3.166,2.882-7.258,4.254-11.501,3.885  c-4.221-0.374-7.968-2.431-10.55-5.789l-51.621-67.136c13.679-9.23,22.989-24.448,24.172-41.861l83.948,11.137  c4.2,0.557,7.854,2.774,10.288,6.242C396.545,269.635,397.392,273.862,396.479,278.045z M475,460h-23.787l-37.42-37.42  c1.498-1.425,2.981-2.871,4.449-4.338C464.516,371.967,490,310.441,490,245s-25.484-126.967-71.759-173.241S310.441,0,245,0  C179.558,0,118.033,25.484,71.759,71.759S0,179.559,0,245s25.484,126.967,71.759,173.241c1.467,1.467,2.951,2.913,4.449,4.338  L38.787,460H15c-8.284,0-15,6.716-15,15s6.716,15,15,15h60c8.284,0,15-6.716,15-15c0-6.434-4.059-11.907-9.75-14.037l18.984-18.984  C141.172,473.151,191.791,490,245,490c53.209,0,103.828-16.849,145.767-48.021l18.984,18.984c-5.691,2.13-9.75,7.603-9.75,14.037  c0,8.284,6.716,15,15,15h60c8.284,0,15-6.716,15-15S483.284,460,475,460z M30,245c0-118.552,96.449-215,215-215  c118.552,0,215,96.448,215,215s-96.448,215-215,215C126.449,460,30,363.552,30,245z",
          "heater":"M9.39,40.32h96.73a9.41,9.41,0,0,1,9.39,9.38v54.81a9.41,9.41,0,0,1-9.39,9.38H94.84V122a.92.92,0,0,1-.92.92H85.35a.93.93,0,0,1-.93-.92v-8.07H31.08V122a.92.92,0,0,1-.92.92H21.59a.93.93,0,0,1-.93-.92v-8.07H9.39A9.39,9.39,0,0,1,0,104.51V49.7a9.39,9.39,0,0,1,9.39-9.38ZM24.66,2.79A2.08,2.08,0,0,1,26.25.11a2.64,2.64,0,0,1,3.17,1.33c3,7.54.85,12.88-1.05,17.58-1.64,4-3,7.49.63,11.38a1.89,1.89,0,0,1-.42,3A2.86,2.86,0,0,1,25.05,33c-5.37-5.71-3.57-10.16-1.46-15.36C25.26,13.59,27.14,9,24.66,2.79ZM72,2.78A2.06,2.06,0,0,1,73.58.11a2.64,2.64,0,0,1,3.18,1.33C79.76,9,77.6,14.32,75.7,19c-1.63,4-3,7.49.63,11.38a1.89,1.89,0,0,1-.42,3A2.84,2.84,0,0,1,72.38,33C67,27.3,68.81,22.85,70.92,17.64,72.59,13.59,74.45,9,72,2.78Zm15.75,0A2.06,2.06,0,0,1,89.33.12a2.64,2.64,0,0,1,3.18,1.32c3,7.55.84,12.89-1.06,17.58-1.63,4-3,7.49.63,11.38a1.89,1.89,0,0,1-.42,3A2.86,2.86,0,0,1,88.13,33c-5.37-5.7-3.57-10.16-1.46-15.36,1.67-4,3.54-8.68,1.08-14.86Zm-47.33,0A2.06,2.06,0,0,1,42,.11a2.64,2.64,0,0,1,3.18,1.33C48.18,9,46,14.32,44.12,19c-1.63,4-3,7.49.63,11.38a1.89,1.89,0,0,1-.42,3A2.84,2.84,0,0,1,40.8,33c-5.37-5.7-3.57-10.15-1.46-15.36C41,13.59,42.87,9,40.42,2.78Zm15.75,0A2.06,2.06,0,0,1,57.75.12a2.64,2.64,0,0,1,3.18,1.32c3,7.55.84,12.89-1.06,17.58-1.63,4-3,7.49.63,11.38a1.89,1.89,0,0,1-.42,3A2.86,2.86,0,0,1,56.55,33c-5.37-5.7-3.57-10.16-1.46-15.36,1.67-4,3.54-8.68,1.08-14.86Zm38,50.37a2.77,2.77,0,0,1,5.53,0v48.13a2.77,2.77,0,1,1-5.53,0V53.16Zm-15.63,0a2.77,2.77,0,0,1,5.53,0v48.13a2.77,2.77,0,1,1-5.53,0V53.16Zm-15.63,0a2.77,2.77,0,0,1,5.53,0v48.13a2.77,2.77,0,1,1-5.53,0V53.16Zm-15.63,0a2.77,2.77,0,0,1,5.53,0v48.13a2.77,2.77,0,1,1-5.53,0V53.16Zm-15.63,0a2.76,2.76,0,0,1,5.52,0v48.13a2.76,2.76,0,1,1-5.52,0V53.16ZM16,53.16a2.76,2.76,0,1,1,5.52,0v48.13a2.76,2.76,0,1,1-5.52,0V53.16Zm90.08-7.5H9.39a4.06,4.06,0,0,0-4,4v54.81a4.06,4.06,0,0,0,4,4h96.73a4.06,4.06,0,0,0,4-4V49.7a4.06,4.06,0,0,0-4-4Z"    
      },
      buttons: []
    }
  };
  
  var cur_lib = library.basic;
  current_d = cur_lib.data.star_points_5

  var mode_id = 'shapelib';
  
  function loadIcons() {
    $('#shape_buttons').empty();
    
    // Show lib ones
    $('#shape_buttons').append(cur_lib.buttons);
  }
  
  function loadLibrary(cat_id) {
  
    var lib = library[cat_id];
    
    if(!lib) {
      $('#shape_buttons').html('Loading...');
      $.getJSON('shapelib/' + cat_id + '.json', function(result, textStatus) {
        cur_lib = library[cat_id] = {
          data: result.data,
          size: result.size,
          fill: result.fill
        }
        makeButtons(cat_id, result);
        loadIcons();
      });
      return;
    }
    
    cur_lib = lib;
    if(!lib.buttons.length) makeButtons(cat_id, lib);
    loadIcons();
  }
  
  function makeButtons(cat, shapes) {
    var size = cur_lib.size || 300;
    var fill = cur_lib.fill || false;
    var off = size * .05;
    var vb = [-off, -off, size + off*2, size + off*2].join(' ');
    var stroke = fill ? 0: (size/30);
    
    var shape_icon = new DOMParser().parseFromString(
      '<svg xmlns="http://www.w3.org/2000/svg"><svg viewBox="' + vb + '"><path /><\/svg><\/svg>',
      'text/xml');

    var width = 40;
    var height = 40;
    shape_icon.documentElement.setAttribute('width', width);
    shape_icon.documentElement.setAttribute('height', height);
    var svg_elem = $(document.importNode(shape_icon.documentElement,true));
  
    var data = shapes.data;
    
    cur_lib.buttons = [];
  
    for(var id in data) {
      var path_d = data[id];
      var icon = svg_elem.clone();
      icon.find('path').attr('d', path_d);
      
      var icon_btn = icon.wrap('<div class="tool_button">').parent().attr({
        id: mode_id + '_' + id,
        title: id
      });
      
      // Store for later use
      cur_lib.buttons.push(icon_btn[0]);
    }
    
  }
  
  return {
    callback: function() {
    
      var btn_div = $('<div id="shape_buttons">');
      $('#tools_shapelib > *').wrapAll(btn_div);
      
      var shower = $('#tool_shapelib');

      loadLibrary('basic');
      
      // Do mouseup on parent element rather than each button
      $('#shape_buttons').mouseup(function(evt) {
        var btn = $(evt.target).closest('div.tool_button');
        
        if(!btn.length) return;
        
        var copy = btn.children().clone().attr({width: 24, height: 24});
        shower.children(':not(.flyout_arrow_horiz)').remove();
        shower
          .append(copy)
          .attr('data-curopt', '#' + btn[0].id) // This sets the current mode
          .mouseup();
        canv.setMode(mode_id);
        
        cur_shape_id = btn[0].id.substr((mode_id+'_').length);
        current_d = cur_lib.data[cur_shape_id];
        
        $('.tools_flyout').fadeOut();

      });

      var shape_cats = $('<div id="shape_cats">');
      var cat_str = '';
      
      $.each(categories, function(id, label) {
        cat_str += '<div data-cat=' + id + '>' + label + '</div>';
      });
      shape_cats.html(cat_str)
      $("[data-cat]", shape_cats)
        .on('click', function(e) {
          var catlink = $(this);
          catlink.siblings().removeClass('current');
          catlink.addClass('current');
          loadLibrary(catlink.attr('data-cat'));
      });
      
      shape_cats.children().eq(0).addClass('current');
      
      $('#tools_shapelib').prepend(shape_cats);

      shower.mouseup(function() {
        canv.setMode(current_d ? mode_id : 'select');
      });

  
    },
    mouseDown: function(opts) {
      
      var mode = canv.getMode();
      if(mode !== mode_id) return;
      
      var e = opts.event;
      var x = start_x = opts.start_x;
      var y = start_y = opts.start_y;
      var cur_style = canv.getStyle();
      cur_shape = canv.addSvgElementFromJson({
        "element": "path",
        "curStyles": true,
        "attr": {
          "d": current_d,
          "id": canv.getNextId(),
          "opacity": cur_style.opacity / 2,
          "style": "pointer-events:none"
        }
      });
      cur_shape.setAttribute("d", current_d);
      // Make sure shape uses absolute values
      if(/[a-z]/.test(current_d)) {
        current_d = cur_lib.data[cur_shape_id] = canv.pathActions.convertPath(cur_shape);
        cur_shape.setAttribute('d', current_d);
        canv.pathActions.fixEnd(cur_shape);
      }
      
      cur_shape.setAttribute('transform', "translate(" + x + "," + y + ") scale(0.005) translate(" + -x + "," + -y + ")");      
      canv.recalculateDimensions(cur_shape);
      var tlist = canv.getTransformList(cur_shape);
      lastBBox = cur_shape.getBBox();
      totalScale = {
        sx: 1,
        sy: 1
      };
      return {
        started: true
      }
      // current_d
    },
    mouseMove: function(opts) {
      var mode = canv.getMode();
      if(mode !== mode_id) return;
      
      var zoom = canv.getZoom();
      var evt = opts.event
      
      var x = opts.mouse_x/zoom;
      var y = opts.mouse_y/zoom;
      
      var tlist = canv.getTransformList(cur_shape),
        box = cur_shape.getBBox(), 
        left = box.x, top = box.y, width = box.width,
        height = box.height;
      var dx = (x-start_x), dy = (y-start_y);

      var newbox = {
        'x': Math.min(start_x,x),
        'y': Math.min(start_y,y),
        'width': Math.abs(x-start_x),
        'height': Math.abs(y-start_y)
      };

      var ts = null,
        tx = 0, ty = 0,
        sy = height ? (height+dy)/height : 1, 
        sx = width ? (width+dx)/width : 1;

      var sx = newbox.width / lastBBox.width;
      var sy = newbox.height / lastBBox.height;
      
      sx = sx || 1;
      sy = sy || 1;
      
      // Not perfect, but mostly works...
      
      if(x < start_x) {
        tx = lastBBox.width;
      }
      if(y < start_y) ty = lastBBox.height;
      
      // update the transform list with translate,scale,translate
      var translateOrigin = svgroot.createSVGTransform(),
        scale = svgroot.createSVGTransform(),
        translateBack = svgroot.createSVGTransform();
        
      translateOrigin.setTranslate(-(left+tx), -(top+ty));
      if(evt.shiftKey) {
        replaced = true
        var max = Math.min(Math.abs(sx), Math.abs(sy));
        sx = max * (sx < 0 ? -1 : 1);
        sy = max * (sy < 0 ? -1 : 1);
        if (totalScale.sx != totalScale.sy) {
          var multiplierX = (totalScale.sx > totalScale.sy) ? 1 : totalScale.sx/totalScale.sy;
          var multiplierY = (totalScale.sy > totalScale.sx) ? 1 : totalScale.sy/totalScale.sx;
          sx *= multiplierY
          sy *= multiplierX
        }
      }
      totalScale.sx *= sx;
      totalScale.sy *= sy;
      scale.setScale(sx,sy);
      translateBack.setTranslate(left+tx, top+ty);
      var N = tlist.numberOfItems;
      tlist.appendItem(translateBack);
      tlist.appendItem(scale);
      tlist.appendItem(translateOrigin);

      canv.recalculateDimensions(cur_shape);
      lastBBox = cur_shape.getBBox();
    },
    mouseUp: function(opts) {
      var mode = canv.getMode();
      if(mode !== mode_id) return;
      
      if(opts.mouse_x == start_x && opts.mouse_y == start_y) {
        return {
          keep: false,
          element: cur_shape,
          started: false
        }
      }

      return {
        keep: true,
        element: cur_shape,
        started: false
      }
    }   
  }
}