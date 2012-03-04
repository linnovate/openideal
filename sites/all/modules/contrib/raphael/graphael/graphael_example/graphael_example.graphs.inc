<?php

/**
 * @file
 * gRaphaël example graphs.
 */

/**
 * Page callback for showing our examples.
 */
function graphael_example_graphs_page() {
  $output = array();

  $popdata = graphael_example_world_population_data();

  $output[] = '<h4>' . t('World population by continent') . '</h4>';

  $output[] = theme('graphael', array(
    'method' => 'bar',
    'values' => $popdata[2000],
    'params' => array(
      'colors' => array('#490A3D', '#BD1550', '#E97F02', '#F8CA00', '#8A9B0F', '#68B3AF'),
      'font' => '10px Arial, sans-serif',
      'opts' => array(
        'gutter' => '80%',
      ),
      'label' => array(
        'values' => array('Africa', 'Asia', 'Europe', 'L. America', 'N. America', 'Oceania'),
        'isBottom' => TRUE,
      ),
    ),
    'extend' => array(
      'label' => array(
        'values' => array('Africa', 'Asia', 'Europe', 'Latin America', 'Northern America', 'Oceania'),
        'params' => array('attrText' => array(
          'fill' =>  '#aaa',
          'font' => '10px Arial, sans-serif',
        )),
      ),
    ),
  ));

  $output[] = '<h4>' . t('World population by continent (hbar)') . '</h4>';

  $output[] = theme('graphael', array(
    'method' => 'hbar',
    'values' => $popdata[2000],
    'params' => array(
      'colors' => array('#490A3D', '#BD1550', '#E97F02', '#F8CA00', '#8A9B0F', '#68B3AF'),
      'font' => '10px Arial, sans-serif',
      'opts' => array(
        'gutter' => '80%',
      ),
      'label' => array(
        'values' => array('Africa', 'Asia', 'Europe', 'L. America', 'N. America', 'Oceania'),
        'isRight' => TRUE,
      ),
    ),
    'extend' => array(
      'label' => array(
        'values' => array('Africa', 'Asia', 'Europe', 'Latin America', 'Northern America', 'Oceania'),
        'params' => array('attrText' => array(
          'fill' =>  '#aaa',
          'font' => '10px Arial, sans-serif',
        )),
      ),
    ),
  ));

  $output[] = '<h4>' . t('World population by continent, grouped bars') . '</h4>';

  $output[] = theme('graphael', array(
    'method' => 'bar',
    'values' => array($popdata[1750], $popdata[1800], $popdata[1850], $popdata[1900], $popdata[1950], $popdata[2000]),
    'params' => array(
      'colors' => array('#490A3D', '#BD1550', '#E97F02', '#F8CA00', '#8A9B0F', '#68B3AF'),
      'font' => '10px Arial, sans-serif',
      'opts' => array(
        'gutter' => '10%',
      ),
      'padding' => 20,    
      // GRaphael breaks when trying to use label data for grouped barcharts.
      // 'label' => array(
      // 'values' => array('Africa', 'Asia', 'Europe', 'L. America', 'N. America', 'Oceania'),
      // 'isBottom' => true,
      // ),
    ),
    'extend' => array(
      'label' => array(
        'values' => array('Africa', 'Asia', 'Europe', 'L. America', 'N. America', 'Oceania'),
        'params' => array(
          'attrText' => array(
            'fill' =>  '#aaa',
            'font' => '12px Arial, sans-serif',
          ),
          'label' => 'label',
          'attrLabel' => array(
            'fill' => '#000',
          ),
        ),
      ),
    ),
  ));
  
  $output[] = '<h4>' . t('Population growth by continent') . '</h4>';

  $x_axis = array_keys($popdata);
  $y_axis = array();
  foreach ($popdata as $year) {
    foreach ($year as $key => $country) {
      $y_axis[$key][] = $country;
    }
  }
  $output[] = theme('graphael', array(
    'method' => 'line',
    'values' => array($x_axis, $y_axis),
    'params' => array(
      'opts' => array(
        'axis' => "0 0 1 1",
        'gutter' => 30,
      )),
    'extend' => array(
      'label' => array(
        'params' => array('attrText' => array(
          'fill' =>  '#aaa',
          'font' => '10px Arial, sans-serif',
        )),
      ),
    ),
    'attr' => array(
      'style' => 'height: 400px;width:600px;',
    ),
  ));

  $output[] = '<h4>' . t('World land mass by continent') . '</h4>';

  $output[] = theme('graphael', array(
    'method' => 'pie',
    'values' => graphael_example_world_landmass_data(),
    'params' => array(
      'colors' => array('#490A3D', '#444', '#BD1550', '#68B3AF', '#E97F02', '#F8CA00', '#8A9B0F'),
      'opts' => array(
        'gutter' => '80%',
        'legend' => array(
          '%%.% – Africa',
          '%%.% – Antarctica',
          '%%.% – Asia',
          '%%.% – Australia',
          '%%.% – Europe',
          '%%.% – South America',
          '%%.% – North America',
        ),
        'legendpos' => 'west',
      ),
    ),
  ));

  $output[] = '<h4>' . t('Progressive dot chart') . '</h4>';

  $output[] = theme('graphael', array(
    'method' => 'dot',
    'values' => graphael_example_fake_dot_data(),
    'params' => array(
      'font' => '9px Arial, sans-serif',
      'opts' => array(
        'symbol' => "f", //  disc, o, flower, f, diamond, d, square, s, triangle, t, star, *, cross, x, plus, +, arrow, ->
        'max' => 12,
        'heat' => TRUE,
        'axis' => "0 0 1 1",
        'axisxstep' => 23,
        'axisystep' => 6,
        'axisylabels'=> array(t('Mon'), t('Tue'), t('Wed'), t('Thu'), t('Fri'), t('Sat'), t('Sun')),
        'axisxlabels' => array(t("12am"), "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", t("12pm"), "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11"),
        'axisxtype' => " ",
        'axisytype' => " ",
      ),
    ),
    'extend' => array(
      'label' => array(
        'params' => array('attrText' => array(
          'fill' =>  '#aaa',
          'font' => '10px Arial, sans-serif',
        )),
      ),
    ),
    'attr' => array(
      'style' => 'height: 300px;width:600px;',
    ),
  ));

  return join($output);
}

/**
 * World population example data.
 */
function graphael_example_world_population_data() {
  return array(
    // Africa, Asia, Europe, Latin America, North America, Oceania
    '1750' => array(106, 502, 163, 16, 2, 2),
    '1800' => array(107, 635, 203, 24, 7, 2),
    '1850' => array(111, 809, 276, 38, 26, 2),
    '1900' => array(133, 947, 408, 74, 82, 6),
    '1950' => array(211, 1398, 547, 167, 172, 12.8),
    '2000' => array(796, 3680, 728, 520, 316, 31),
  );
}

/**
 * World landmass example data.
 */
function graphael_example_world_landmass_data() {
  // Africa, Antarctica, Asia, Australia, Europe, South America, North America
  return array(30370000, 13720000, 43820000, 9008500, 10180000, 17840000, 24490000);
}

function graphael_example_fake_dot_data() {
  $x = array(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23);
  $y = array(7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
  $aplityde = range(1,168);
  return array($x, $y, $aplityde);
}

