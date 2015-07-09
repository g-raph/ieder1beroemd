/**
 * @file
 * JavaScript behaviors for the front-end display of webforms.
 */

(function ($) {

  "use strict";

  Drupal.behaviors.webform_ytpicker = Drupal.behaviors.webform_ytpicker || {};

  Drupal.behaviors.webform_ytpicker.attach = function (context) {
    console.log('webform_ytpicker');
    $.each($('input.webform_ytpicker'),function(index,item){
      var $item = $(item);
      var newhtml = Drupal.settings.webform_ytpicker.iframe_template;
      var ytVidId = $item.val();
      newhtml = newhtml.replace('{vid_id}', ytVidId);
      $item.parent().find('.youtubeframe').remove();
      $item.parent().append(newhtml);
    });
  };

  Drupal.webform_ytpicker = Drupal.webform_ytpicker || {};


})(jQuery);
