<?php
/**
 * @file
 * The actiual component hooks
 */

/**
 * Implements hook_webform_defaults_component().
 */
function _webform_defaults_ytpicker() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'pid' => 0,
    'weight' => 0,
    'value' => '',
    'mandatory' => 0,
    'extra' => array(
      'yt_amount' => '',
      'yt_vid_0' => '',
      'analysis' => TRUE,
    ),
  );
}

/**
 * Implements hook_webform_theme_component().
 */
function _webform_theme_ytpicker() {
  return array(
    'webform_display_ytpicker' => array(
      'render element' => 'element',
    ),
  );
}

/**
 * Implements hook_webform_edit_component().
 */
function _webform_edit_ytpicker($component) {
  $form = webform_ytpicker_component_form($component);
  return $form;
}


/**
 * Implements hook_webform_render_component().
 */
function _webform_render_ytpicker($component, $value = NULL, $filter = TRUE) {
  $node = isset($component['nid']) ? node_load($component['nid']) : NULL;
  $videos = _webform_ytpicker_options($component, 'render');
  _webform_ytpicker_add_analysis_data($videos, $component);
  $width = $component['extra']['yt']['width'];
  $height = $component['extra']['yt']['height'];
  $iframe_tpl = theme('webform_ytpicker_iframe', array(
    'width' => $width,
    'height' => $height,
    'vidId' => '{vid_id}',
    ));
  $sets = array(
    'webform_ytpicker' => array(
      'components' => array(
        $component['form_key'] => array(
          'source' => $component['extra']['yt'],
          'videos' => $videos,
        ),
      ),
      'iframe_template' => $iframe_tpl,
    ),
  );
  drupal_add_js($sets, 'setting');
  drupal_add_js(drupal_get_path('module', 'webform_ytpicker') . '/js/webform_ytpicker.js', 'file');
  foreach ($videos as $video) {
    $options[$video->ytid] = $video->name;
  }
  $element = array(
    '#title' => $component['name'],
    '#type' => 'radios',
    '#options' => $options,
    '#required' => $component['mandatory'],
    '#weight' => $component['weight'],
    '#attributes' => array(
      'class' => array('webform_ytpicker'),
    ),
    '#theme_wrappers' => array('radios'),
    '#process' => array_merge(element_info_property('radios', '#process'), array('webform_expand_select_ids')),
  );
  return $element;
}


/**
 * Extract the video Ifrom the youtube url.
 */
function _webform_ytpicker_video_id($url) {
  $patterns = array(
    '@youtube\.com/watch[#\?].*?v=([^"\& ]+)@i',
    '@youtube\.com/embed/([^"\&\? ]+)@i',
    '@youtube\.com/v/([^"\&\? ]+)@i',
    '@youtube\.com/\?v=([^"\& ]+)@i',
    '@youtu\.be/([^"\&\? ]+)@i',
    '@gdata\.youtube\.com/feeds/api/videos/([^"\&\? ]+)@i',
  );
  foreach ($patterns as $pattern) {
    preg_match($pattern, $url, $matches);
    if (isset($matches[1])) {
      $vid_id = $matches[1];
      return $vid_id;
    }
  }
}

/**
 * Implements hook_webform_display_component().
 */
function _webform_display_ytpicker($component, $value, $format = 'html') {
  return array(
    '#title' => $component['name'],
    '#weight' => $component['weight'],
    '#theme' => 'webform_display_ytpicker',
    '#theme_wrappers' => $format == 'html' ? array('webform_element') : array('webform_element_text'),
    '#format' => $format,
    '#value' => $value[0],
    '#translatable' => array('title', 'field_prefix', 'field_suffix'),
    '#component' => $component,
  );
}

/**
 * Implements hook_webform_display_component().
 */
function theme_webform_display_ytpicker($variables) {
  $element = $variables['element'];
  $component = $element['#webform_component'];
  $videos = _webform_ytpicker_options($component, 'display');
  $width = $component['extra']['yt']['width'];
  $height = $component['extra']['yt']['height'];

  $video = _webform_ytpicker_get_by_ytib($element['#value'], $videos);
  $iframe_tpl = theme('webform_ytpicker_iframe', array(
    'width' => $width,
    'height' => $height,
    'vidId' => $video->ytid,
    ));
  return $iframe_tpl;
}


/**
 * Implements hook_webform_analysis_component().
 */
function _webform_analysis_ytpicker($component, $sids = array(), $single = FALSE, $join = NULL) {
  $videos = _webform_ytpicker_options($component, 'analysis');
  // Create a generic query for the component.
  $query = db_select('webform_submitted_data', 'wsd', array('fetch' => PDO::FETCH_ASSOC))
    ->condition('wsd.nid', $component['nid'])
    ->condition('wsd.cid', $component['cid'])
    ->condition('wsd.data', '', '<>');

  if ($sids) {
    $query->condition('wsd.sid', $sids, 'IN');
  }

  if ($join) {
    $query->innerJoin($join, 'ws2_', 'wsd.sid = ws2_.sid');
  }

  $rows = array();
  $other = array();
  $normal_count = 0;
  foreach ($videos as $video) {
    $options[] = $video->ytid;
  }
  if ($options) {
    // Gather the normal results first (not "other" options).
    $query->addExpression('COUNT(wsd.data)', 'datacount');
    $result = $query
      ->condition('wsd.data', $options, 'IN')
      ->fields('wsd', array('data'))
      ->groupBy('wsd.data')
      ->execute();
    foreach ($result as $data) {
      if (isset($data['data'])) {
        $video = _webform_ytpicker_get_by_ytib($data['data'], $videos);
        $display_option = $video->name;
        $rows[$data['data']] = array(webform_filter_xss($display_option), $data['datacount']);
        $normal_count += $data['datacount'];
      }
    }
  }

  return array(
    'table_rows' => $rows,
    'other_data' => $other,
  );
}

/**
 * Helper function.
 */
function _webform_ytpicker_get_by_ytib($id, $videos) {
  foreach ($videos as $video) {
    if ($video->ytid == $id) {
      return $video;
    }
  }
}

/**
 * Implements hook_webform_table_component().
 */
function _webform_table_ytpicker($component, $value) {
  $videos = _webform_ytpicker_options($component, 'table');
  $video = _webform_ytpicker_get_by_ytib($value[0], $videos);
  return check_plain(empty($video->name) ? '' : $video->name);
}

/**
 * Implements hook_webform_headers_component().
 */
function _webform_csv_headers_ytpicker($component, $export_options) {
  $header = array();
  $header[0] = '';
  $header[1] = $component['name'];
  return $header;
}

/**
 * Implements hook_webform_csv_data_component().
 */
function _webform_csv_data_ytpicker($component, $export_options, $value) {
  $videos = _webform_ytpicker_options($component, 'csv');
  $video = _webform_ytpicker_get_by_ytib($value[0], $videos);
  return !isset($video->name) ? '' : $video->name;
}

/**
 * Adds amount of votes to Javascript array.
 */
function _webform_ytpicker_add_analysis_data(&$videos, $component) {
  $rows = array();
  $other = array();
  $normal_count = 0;
  $options = array();
  // if(empty($options)) {
  //   return array();
  // }
  foreach ($videos as $video) {
    $options[] = $video->ytid;
  }
  $query = db_select('webform_submitted_data', 'wsd', array('fetch' => PDO::FETCH_ASSOC))
    ->condition('wsd.nid', $component['nid'])
    ->condition('wsd.cid', $component['cid'])
    ->condition('wsd.data', '', '<>');
  $query->addExpression('COUNT(wsd.data)', 'datacount');
  $result = $query
    ->condition('wsd.data', $options, 'IN')
    ->fields('wsd', array('data'))
    ->groupBy('wsd.data')
    ->execute();
  $total = 0;
  $result_set = array();
  foreach ($result as $data) {
    $result_set[] = $data;
    $total  = $total + $data['datacount'];
  }

  foreach ($videos as $key => &$video) {
    foreach ($result_set as $data) {
      if (isset($data['data']) && $data['data'] == $video->ytid) {
        $video->thumbnail = 'http://img.youtube.com/vi/' . $video->ytid . '/0.jpg';
        $display_option = $video->name;
        $rows[$data['data']] = array(webform_filter_xss($display_option), $data['datacount']);
        $normal_count += $data['datacount'];
        $video->analysis = new stdClass();
        $video->analysis->count = intval($data['datacount']);
        $video->analysis->total = intval($total);
      }
    }
  }
}


/**
 * Generate a list of options for a select list.
 */
function _webform_ytpicker_options($component, $hook) {
  if ($component['extra']['yt']['use_nodes']) {
    $field = field_info_field($component['extra']['yt']['nodes_field']);
    $table_name = array_keys($field['storage']['details']['sql']['FIELD_LOAD_CURRENT'])[0];
    $query = 'select * from '.$table_name.' where 1';
    $res = db_query($query, array());
    $i = 0;

    foreach ($res as $item) {
      $node = node_load($item->entity_id);
      $vid = new stdClass();
      $vid->id = $i;
      $vid->bundle = $item->bundle;
      $vid->entity_type = $item->entity_type;
      $vid->entity_id = $item->entity_id;
      $vid->name = $node->title;
      $vid->url = $item->field_youtube_video_input;
      $vid->ytid = $item->field_youtube_video_video_id;
      $videos[] = $vid;
      $i++;
    }
  }else{
    $videos = array();
    $lines = explode(PHP_EOL, $component['extra']['yt']['ytp']);
    $i = 0;
    foreach ($lines as $line) {
      $exp = explode('|', $line);
      $vid = new stdClass();
      $vid->id = $i;
      $vid->name = $exp[0];
      $vid->url = $exp[1];
      $vid->ytid = _webform_ytpicker_video_id(trim($exp[1]));
      $videos[] = $vid;
      $i++;
    }
  }
  drupal_alter('webform_ytpicker_videos_list', $videos, $hook);
  return $videos;
}
