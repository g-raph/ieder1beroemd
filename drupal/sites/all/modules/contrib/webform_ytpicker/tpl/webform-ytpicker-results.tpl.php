<div class="<?php print $classes; ?>">
  <div class="webform-analysis-component-inner">
    <h3><?php print check_plain($component['name']); ?></h3>
    <?php print drupal_render_children($component_analysis); ?>
  </div>
</div>