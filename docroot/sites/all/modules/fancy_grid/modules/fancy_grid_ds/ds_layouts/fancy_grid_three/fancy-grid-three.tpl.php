<?php
/**
 * @file
 * Display Suite Fancy Grid Three template.
 *
 * Available variables:
 *
 * Layout:
 * - $classes: String of classes that can be used to style this layout.
 * - $contextual_links: Renderable array of contextual links.
 * - $layout_wrapper: wrapper surrounding the layout.
 *
 * Regions:
 *
 * - $one: Rendered content for the "One" region.
 * - $one_classes: String of classes that can be used to style the "One" region.
 *
 * - $two: Rendered content for the "Two" region.
 * - $two_classes: String of classes that can be used to style the "Two" region.
 *
 * - $three: Rendered content for the "Three" region.
 * - $three_classes: String of classes that can be used to style the "Three" region.
 */
?>
<<?php print $layout_wrapper; print $layout_attributes; ?> class="<?php print $classes;?> clearfix">

  <?php if(isset($extra_wrapper) && $extra_wrapper): ?>
    <div class="<?php print $extra_wrapper_classes;?>">
  <?php endif; ?>

    <?php if (isset($title_suffix['contextual_links'])): ?>
      <?php print render($title_suffix['contextual_links']); ?>
    <?php endif; ?>

    <<?php print $one_wrapper; ?> class="ds-one ds-region<?php print $one_classes; ?>">
      <?php print $one; ?>
    </<?php print $one_wrapper; ?>>

    <<?php print $two_wrapper; ?> class="ds-two ds-region<?php print $two_classes; ?>">
      <?php print $two; ?>
    </<?php print $two_wrapper; ?>>

    <<?php print $three_wrapper; ?> class="ds-three ds-region<?php print $three_classes; ?>">
      <?php print $three; ?>
    </<?php print $three_wrapper; ?>>

  <?php if(isset($extra_wrapper) && $extra_wrapper): ?>
    </div>
  <?php endif; ?>

</<?php print $layout_wrapper ?>>