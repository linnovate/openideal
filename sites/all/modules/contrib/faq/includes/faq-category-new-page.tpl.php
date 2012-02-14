<?php
// $Id: faq-category-new-page.tpl.php,v 1.1.2.3 2009/02/08 17:24:49 snpower Exp $

/**
 * @file
 * Template file for the FAQ page if set to show the categorized answer in a
 * new page.
 */

/**
 * Available variables:
 *
 * $display_header
 *   Boolean value controlling whether a header should be displayed.
 * $header_title
 *   The category title.
 * $category_depth
 *   The term or category depth.
 * $description
 *   The current page's description.
 * $term_image
 *   The HTML for the category image. This is empty if the taxonomy image module
 *   is not enabled or there is no image associated with the term.
 * $display_faq_count
 *   Boolean value controlling whether or not the number of faqs in a category
 *   should be displayed.
 * $question_count
 *   The number of questions in category.
 * $question_list
 *   An array of questions.
 * $question_list_style, either ol or ul
 *   The style of the question list, either ol or ul (ordered or unordered).
 * $container_class
 *   The class attribute of the element containing the sub-categories, either
 *   'faq-qa' or 'faq-qa-hide'. This is used by javascript to open/hide
 *   a category's faqs.
 * $subcat_list
 *   An array of sub-categories.  Each sub-category stored in the $subcat_list
 *   array has the following information:
 *     $subcat['link'] is the link to the sub-category.
 *     $subcat['description'] is the sub-category description.
 *     $subcat['count'] is the number of questions in the sub-category.
 *     $subcat['term_image'] is the sub-category (taxonomy) image.
 * $subcat_list_style
 *   The style of the sub-category list, either ol or ul (ordered or unordered).
 * $subcat_body_list
 *   The sub-categories faqs, recursively themed (by this template).
 */

if ($category_depth > 0) {
  $hdr = 'h6';
}
else {
  $hdr = 'h5';
}

?><div class="faq-category-group">
  <!-- category header with title, link, image, description, and count of
  questions inside -->
  <div class="faq-qa-header">
  <?php if ($display_header): ?>
    <<?php print $hdr; ?> class="faq-header">
    <?php print $term_image; ?>
    <?php print $header_title; ?>
    <?php if ($display_faq_count): ?>
      (<?php print $question_count; ?>)
    <?php endif; ?>
    </<?php print $hdr; ?>>

  <?php else: ?>
    <?php print $term_image; ?>
  <?php endif; ?>

  <?php if (!empty($description)): ?>
    <div class="faq-qa-description"><?php print $description ?></div>
  <?php endif; ?>
  <?php if (!empty($term_image)): ?>
    <div class="clear-block"></div>
  <?php endif; ?>
  </div> <!-- Close div: faq-qa-header -->

  <!-- list subcategories, with title, link, description, count -->
  <?php if (!empty($subcat_list)): ?>
    <div class="item-list">
    <<?php print $subcat_list_style; ?> class="faq-category-list">
    <?php foreach ($subcat_list as $i => $subcat): ?>
      <li>
      <?php print $subcat['link']; ?>
      <?php if ($display_faq_count): ?>
        (<?php print $subcat['count']; ?>)
      <?php endif; ?>
      <?php if (!empty($subcat['description'])): ?>
      <div class="faq-qa-description"><?php print $subcat['description']; ?></div>
      <?php endif; ?>
      <div class="clear-block"></div>
      </li>
    <?php endforeach; ?>
    </<?php print $subcat_list_style; ?>>
  </div> <!-- Close div: item-list -->
  <?php endif; ?>

  <div class="<?php print $container_class; ?>">

  <!-- include subcategories -->
  <?php if (count($subcat_body_list)): ?>
    <?php foreach ($subcat_body_list as $i => $subcat_html): ?>
      <div class="faq-category-indent"><?php print $subcat_html; ?></div>
    <?php endforeach; ?>
  <?php endif; ?>

  <!-- list questions (in title link) and answers (in body) -->
  <?php if (!empty($question_list)): ?>
    <div class="item-list">
    <<?php print $question_list_style; ?> class="faq-category-list">
    <?php foreach ($question_list as $i => $question_link): ?>
      <li>
      <?php print $question_link; ?>
      </li>
    <?php endforeach; ?>
    </<?php print $question_list_style; ?>>
  </div> <!-- Close div: item-list -->
  <?php endif; ?>


  </div> <!-- Close div: faq-qa / faq-qa-hide -->

</div> <!-- Close div: faq-category-group -->
