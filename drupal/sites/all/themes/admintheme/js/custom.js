(function ($) {
    Drupal.behaviors.admintheme = {
        attach: function (context, settings) {


            // dropdowns
            var secmenubutton = $('.navbar-fixed-top a.menu-sec-button');
            secmenubutton.click(function(){
                $(this).toggleClass('open');
                $(this).next().fadeToggle(200);
            });
            var loginmenubutton = $('.navbar-fixed-top a.menu-login-button');
            loginmenubutton.click(function(){
                $(this).toggleClass('open');
                $(this).next().fadeToggle(200);
            });

            // sidebar toggle
            var $sidebar = $('.main-sidebar');
            var $togglebutton = $sidebar.find('> i.togglebutton');
            var $closebutton = $sidebar.find('> i.closebutton');
            $togglebutton.click(function(){
                $sidebar.toggleClass('closed');
                $sidebar.toggleClass('open');
                $('#main').addClass('closed');
            });
            $closebutton.click(function(){
                $sidebar.addClass('closed');
                $sidebar.removeClass('open');
                $('#main').addClass('closed');
            });
            $(window).bind('load resize',function(){
                var ww = $(window).width();
                if (ww < 980) {
                    $sidebar.addClass('closed');
                    $sidebar.removeClass('open');
                    $('.main-container').addClass('closed');
                } else {
                    $sidebar.removeClass('closed');
                    $sidebar.addClass('open');
                    $('.main-container').removeClass('closed');
                }
            });

            // main menu
            $('.main-sidebar ul.menu.navbar-nav > li.dropdown > a').removeAttr('data-target data-toggle');
            var mainsubmenu = $('.main-sidebar ul.menu.navbar-nav > li > ul');
            mainsubmenu.hide();
            var lidrop = $('.main-sidebar ul.menu.navbar-nav > li.dropdown');
            lidrop.each(function(){
                var $this = $(this);
                $('<i class="fa fa-chevron-down"></i>').appendTo($this);
                $this.find('> i').click(function(){
                    $this.find('> ul').slideToggle(200);
                    $this.find('> i').toggleClass('open');
                });
            });


        }
    };
})(jQuery);