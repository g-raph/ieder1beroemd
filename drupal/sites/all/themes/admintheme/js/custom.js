(function ($) {
    Drupal.behaviors.admintheme = {
        attach: function (context, settings) {

            // Thumbs for vote videos
            $('#edit-submitted-pick-a-video > .form-item').each(function(index){
                var $this = $(this);
                var thumbUrl = Drupal.settings.webform_ytpicker.components.pick_a_video.videos[index].thumbnail;
                $('<img class="video-thumb" src="'+thumbUrl+'">').insertBefore($this.find('> iframe'));
                $this.find('.video-thumb').click(function(){
                    $this.siblings().find('> iframe').fadeOut(200);
                    $(this).next().fadeIn(200);
                });
            });

            // Hall of fame slider
            $('#block-iedereenberoemd-iedereemberoemd-winners .nodeslist > .item-list > ul').addClass('slider');
            $('#block-iedereenberoemd-iedereemberoemd-winners .nodeslist > .item-list').flexslider({
                selector: "ul.slider > li",
                animation: 'slide',
                minItems: 1,
                maxItems: 5,
                itemWidth: 152,
                itemMargin: 0,
                controlNav: false,
                move: 1
            });


        }
    };
})(jQuery);