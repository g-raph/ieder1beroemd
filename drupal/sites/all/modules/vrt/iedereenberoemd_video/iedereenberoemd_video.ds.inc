<?php
/**
 * @file
 * iedereenberoemd_video.ds.inc
 */

/**
 * Implements hook_ds_field_settings_info().
 */
function iedereenberoemd_video_ds_field_settings_info() {
  $export = array();

  $ds_fieldsetting = new stdClass();
  $ds_fieldsetting->api_version = 1;
  $ds_fieldsetting->id = 'node|video|teaser';
  $ds_fieldsetting->entity_type = 'node';
  $ds_fieldsetting->bundle = 'video';
  $ds_fieldsetting->view_mode = 'teaser';
  $ds_fieldsetting->settings = array(
    'title' => array(
      'weight' => '1',
      'label' => 'hidden',
      'format' => 'default',
    ),
    'iedereenberoemd_winner_votes' => array(
      'weight' => '3',
      'label' => 'hidden',
      'format' => 'default',
    ),
    'iedereenberoemd_yt_thumb' => array(
      'weight' => '4',
      'label' => 'hidden',
      'format' => 'default',
    ),
  );
  $export['node|video|teaser'] = $ds_fieldsetting;

  return $export;
}

/**
 * Implements hook_ds_layout_settings_info().
 */
function iedereenberoemd_video_ds_layout_settings_info() {
  $export = array();

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|video|teaser';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'video';
  $ds_layout->view_mode = 'teaser';
  $ds_layout->layout = 'ds_1col';
  $ds_layout->settings = array(
    'hide_empty_regions' => 0,
    'hide_sidebars' => 0,
    'regions' => array(
      'ds_content' => array(
        0 => 'field_youtube_video',
        1 => 'title',
        2 => 'body',
        3 => 'iedereenberoemd_winner_votes',
        4 => 'iedereenberoemd_yt_thumb',
      ),
    ),
    'fields' => array(
      'field_youtube_video' => 'ds_content',
      'title' => 'ds_content',
      'body' => 'ds_content',
      'iedereenberoemd_winner_votes' => 'ds_content',
      'iedereenberoemd_yt_thumb' => 'ds_content',
    ),
    'classes' => array(),
  );
  $export['node|video|teaser'] = $ds_layout;

  return $export;
}
