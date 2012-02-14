<?php
// $Id: print_pdf.admin.inc,v 1.1.2.29 2010/08/19 23:28:32 jcnventura Exp $

/**
 * @file
 * Contains the administrative functions of the PDF version module.
 *
 * This file is included by the PDF version module, and includes the
 * settings form.
 *
 * @ingroup print
 */

/**
 * Menu callback for the PDF version module settings form.
 *
 * @ingroup forms
 * @see _print_pdf_tools()
 */
function print_pdf_settings() {
  $pdf_tools = _print_pdf_tools();

  if ($pdf_tools != -1) {
    $form['settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('PDF options'),
    );

    $form['settings']['print_pdf_pdf_tool'] = array(
      '#type' => 'radios',
      '#title' => t('PDF generation tool'),
      '#options' => $pdf_tools,
      '#default_value' => variable_get('print_pdf_pdf_tool', PRINT_PDF_PDF_TOOL_DEFAULT),
      '#description' => t('This option selects the PDF generation tool being used by this module to create the PDF version.'),
    );

    $form['settings']['print_pdf_link_pos'] = array(
      '#type' => 'checkboxes',
      '#title' => t('PDF version link'),
      '#default_value' => variable_get('print_pdf_link_pos', array(PRINT_PDF_LINK_POS_DEFAULT => PRINT_PDF_LINK_POS_DEFAULT)),
      '#options' => array('link' => t('Links area'), 'corner' => t('Content corner')),
      '#description' => t('Choose the location of the link(s) to the PDF version. The Links area is usually below the node content, whereas the Content corner is placed in the upper-right corner of the node content. Unselect all options to disable the link. Even if the link is disabled, you can still view the PDF version of a node by going to !path/nid where nid is the numeric id of the node.', array('!path' => PRINTPDF_PATH)),
    );

    $form['settings']['print_pdf_link_teaser'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display link to the PDF version in teaser'),
      '#default_value' => variable_get('print_pdf_link_teaser', PRINT_PDF_LINK_TEASER_DEFAULT),
      '#description' => t('Enabling this will display the link in teaser mode.'),
    );

    $form['settings']['adv_link'] = array(
      '#type' => 'fieldset',
      '#title' => t('Advanced link options'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );

    $form['settings']['adv_link']['print_pdf_show_link'] = array(
      '#type' => 'radios',
      '#title' => t('Link style'),
      '#default_value' => variable_get('print_pdf_show_link', PRINT_PDF_SHOW_LINK_DEFAULT),
      '#options' => array(1 => t('Text only'), 2 => t('Icon only'), 3 => t('Icon and Text')),
      '#description' => t('Select the visual style of the link.'),
    );

    $form['settings']['adv_link']['print_pdf_link_use_alias'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use URL alias instead of node ID'),
      '#default_value' => variable_get('print_pdf_link_use_alias', PRINT_PDF_LINK_USE_ALIAS_DEFAULT),
      '#description' => t('Enabling this will create the link using the URL alias instead of the node ID.'),
    );

    $form['settings']['adv_link']['print_pdf_link_class'] = array(
      '#type' => 'textfield',
      '#title' => t('Link class'),
      '#default_value' => variable_get('print_pdf_link_class', PRINT_PDF_LINK_CLASS_DEFAULT),
      '#size' => 60,
      '#maxlength' => 250,
      '#description' => t('This can be used by themers to change the link style or by jQuery modules to open in a new window (e.g. greybox or thickbox). Multiple classes can be specified, separated by spaces.'),
    );

    $form['settings']['adv_link']['print_pdf_node_link_visibility'] = array(
      '#type' => 'radios',
      '#title' => t('Link visibility'),
      '#default_value' => variable_get('print_pdf_node_link_visibility', PRINT_PDF_NODE_LINK_VISIBILITY_DEFAULT),
      '#options' => array(t('Show on every page except the listed pages.'), t('Show on only the listed pages.')),
    );

    $form['settings']['adv_link']['print_pdf_node_link_pages'] = array(
      '#type' => 'textarea',
      '#default_value' => variable_get('print_pdf_node_link_pages', PRINT_PDF_NODE_LINK_PAGES_DEFAULT),
      '#rows' => 3,
      '#description' => t("Enter one page per line as Drupal paths. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>')),
    );

    $access = user_access('use PHP for link visibility');

    if ($form['settings']['adv_link']['print_pdf_node_link_visibility']['#default_value'] == 2 && !$access) {
      $form['settings']['adv_link']['print_pdf_node_link_visibility'] = array('#type' => 'value', '#value' => 2);
      $form['settings']['adv_link']['print_pdf_node_link_pages'] = array('#type' => 'value', '#value' => $form['settings']['adv_link']['print_pdf_node_link_pages']['#default_value']);
    }
    elseif ($access) {
      $form['settings']['adv_link']['print_pdf_node_link_visibility']['#options'][] = t('Show if the following PHP code returns <code>TRUE</code> (PHP-mode, experts only).');
      $form['settings']['adv_link']['print_pdf_node_link_pages']['#description'] .= ' '. t('If the PHP-mode is chosen, enter PHP code between %php. Note that executing incorrect PHP-code can break your Drupal site.', array('%php' => '<?php ?>'));
    }

    $form['settings']['adv_link']['print_pdf_sys_link_visibility'] = array(
      '#type' => 'radios',
      '#title' => t('Show link in system (non-content) pages'),
      '#default_value' => variable_get('print_pdf_sys_link_visibility', PRINT_PDF_SYS_LINK_VISIBILITY_DEFAULT),
      '#options' => array(t('Show on every page except the listed pages.'), t('Show on only the listed pages.')),
    );

    $form['settings']['adv_link']['print_pdf_sys_link_pages'] = array(
      '#type' => 'textarea',
      '#default_value' => variable_get('print_pdf_sys_link_pages', PRINT_PDF_SYS_LINK_PAGES_DEFAULT),
      '#rows' => 3,
      '#description' => t('Setting this option will add a PDF version page link on pages created by Drupal or the enabled modules.') .'<br />'.
                        t("Enter one page per line as Drupal paths. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>')),
    );

    if ($form['settings']['adv_link']['print_pdf_sys_link_visibility']['#default_value'] == 2 && !$access) {
      $form['settings']['adv_link']['print_pdf_sys_link_visibility'] = array('#type' => 'value', '#value' => 2);
      $form['settings']['adv_link']['print_pdf_sys_link_pages'] = array('#type' => 'value', '#value' => $form['settings']['adv_link']['print_pdf_sys_link_pages']['#default_value']);
    }
    elseif ($access) {
      $form['settings']['adv_link']['print_pdf_sys_link_visibility']['#options'][] = t('Show if the following PHP code returns <code>TRUE</code> (PHP-mode, experts only).');
      $form['settings']['adv_link']['print_pdf_sys_link_pages']['#description'] .= ' '. t('If the PHP-mode is chosen, enter PHP code between %php. Note that executing incorrect PHP-code can break your Drupal site.', array('%php' => '<?php ?>'));
    }

    $form['settings']['adv_link']['print_pdf_book_link'] = array(
      '#type' => 'radios',
      '#title' => t('Link in book hierarchy nodes'),
      '#default_value' => variable_get('print_pdf_book_link', PRINT_PDF_BOOK_LINK_DEFAULT),
      '#options' => array(t('No link'), t('Current page and sub-pages'), t('Current page only')),
    );

    $form['settings']['print_pdf_content_disposition'] = array(
      '#type' => 'radios',
      '#title' => t('Open PDF in'),
      '#options' => array(t('Same browser window'), t('New browser window'), t('Save dialog')),
      '#default_value' => variable_get('print_pdf_content_disposition', PRINT_PDF_CONTENT_DISPOSITION_DEFAULT),
      '#description' => t("Select the desired method for opening the PDF in the user's browser."),
    );

    $form['settings']['print_pdf_paper_size'] = array(
      '#type' => 'select',
      '#title' => t('Paper size'),
      '#options' => array('4A0' => '4A0', '2A0' => '2A0', 'A0' => 'A0',
                          'A1' => 'A1', 'A2' => 'A2', 'A3' => 'A3', 'A4' => 'A4',
                          'A5' => 'A5', 'A6' => 'A6', 'A7' => 'A7', 'A8' => 'A8',
                          'A9' => 'A9', 'A10' => 'A10', 'B0' => 'B0', 'B1' => 'B1',
                          'B2' => 'B2', 'B3' => 'B3', 'B4' => 'B4', 'B5' => 'B5',
                          'B6' => 'B6', 'B7' => 'B7', 'B8' => 'B8', 'B9' => 'B9',
                          'B10' => 'B10', 'C0' => 'C0', 'C1' => 'C1', 'C2' => 'C2',
                          'C3' => 'C3', 'C4' => 'C4', 'C5' => 'C5', 'C6' => 'C6',
                          'C7' => 'C7', 'C8' => 'C8', 'C9' => 'C9', 'C10' => 'C10',
                          'RA0' => 'RA0', 'RA1' => 'RA1', 'RA2' => 'RA2',
                          'RA3' => 'RA3', 'RA4' => 'RA4', 'SRA0' => 'SRA0',
                          'SRA1' => 'SRA1', 'SRA2' => 'SRA2', 'SRA3' => 'SRA3',
                          'SRA4' => 'SRA4', 'LETTER' => 'Letter', 'LEGAL' => 'Legal',
                          'EXECUTIVE' => 'Executive', 'FOLIO' => 'Folio',
                    ),
      '#default_value' => variable_get('print_pdf_paper_size', PRINT_PDF_PAPER_SIZE_DEFAULT),
      '#description' => t('Choose the paper size of the generated PDF.'),
    );

    $form['settings']['print_pdf_page_orientation'] = array(
      '#type' => 'select',
      '#title' => t('Page orientation'),
      '#options' => array('portrait' => t('Portrait'), 'landscape' => t('Landscape')),
      '#default_value' => variable_get('print_pdf_page_orientation', PRINT_PDF_PAGE_ORIENTATION_DEFAULT),
      '#description' => t('Choose the page orientation of the generated PDF.'),
    );
    $form['settings']['print_pdf_autoconfig'] = array(
      '#type' => 'checkbox',
      '#title' => t('Auto-configure the PDF tool settings'),
      '#default_value' => variable_get('print_pdf_autoconfig', PRINT_PDF_AUTOCONFIG_DEFAULT),
      '#description' => t('If you disable this option, the pdf tool settings must be configured manually. For TCDPF, edit the tcpdf/config/tcpdf_config.php file. For dompdf, edit the dompdf/dompdf_config.inc.php file.'),
    );
    $form['settings']['print_pdf_font_family'] = array(
      '#type' => 'textfield',
      '#title' => t('Font Family'),
      '#default_value' => variable_get('print_pdf_font_family', PRINT_PDF_FONT_FAMILY_DEFAULT),
      '#size' => 60,
      '#maxlength' => 250,
      '#description' => t('(TCPDF only) Set the font family to be used. Examples: %examples.', array('%examples' => 'helvetica, times, courier, dejavusans, dejavuserif, freesans, freeserif, freemono')) .'<br />'.
                        t("CAUTION: TCPDF embeds the complete font in the generated PDF. If you're not using Unicode, then helvetica or times are safe choices that will keep the PDF small. Unicode fonts can increase the size of the PDF to the 1MB region."),
    );
    $form['settings']['print_pdf_font_size'] = array(
      '#type' => 'textfield',
      '#title' => t('Font Size'),
      '#default_value' => variable_get('print_pdf_font_size', PRINT_PDF_FONT_SIZE_DEFAULT),
      '#size' => 2,
      '#maxlength' => 3,
      '#description' => t('(TCPDF only) Set the font size to be used for normal text. This is the base value for the scaling applied to other text styles.'),
    );
    $form['settings']['print_pdf_dompdf_unicode'] = array(
      '#type' => 'checkbox',
      '#title' => t("Use dompdf's Unicode Mode"),
      '#default_value' => variable_get('print_pdf_dompdf_unicode', PRINT_PDF_DOMPDF_UNICODE_DEFAULT),
      '#description' => t("If enabled, dompdf's Unicode mode is used. If not, the module will attempt to convert some non-ASCII chars to ISO-8859-1."),
    );
    $form['settings']['print_pdf_wkhtmltopdf_options'] = array(
      '#type' => 'textfield',
      '#title' => t('wkhtmltopdf options'),
      '#size' => 60,
      '#maxlength' => 250,
      '#default_value' => variable_get('print_pdf_wkhtmltopdf_options', PRINT_PDF_WKHTMLTOPDF_OPTIONS),
      '#description' => t('(wkhtmltopdf only) Set any additional options to be passed to the wkhtmltopdf executable. Tokens may be used in these options (see list below).'),
    );

    if (module_exists('token')) {
      $form['settings']['print_pdf_filename'] = array(
        '#type' => 'textfield',
        '#title' => t('PDF File Name'),
        '#default_value' => variable_get('print_pdf_filename', PRINT_PDF_FILENAME_DEFAULT),
        '#description' => t("If left empty the generated filename defaults to the node's path. Tokens may be used to build the filename (see following list). The .pdf extension will be appended automatically."),
      );
      $form['settings']['print_pdf_filename_patterns'] = array(
        '#type' => 'fieldset',
        '#title' => t('Replacement Patterns'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );
      $form['settings']['print_pdf_filename_patterns']['descriptions'] = array(
        '#type' => 'markup',
        '#value' => theme('token_help', array('node', 'global')),
      );
    }
    $form['#validate'][] = '_print_pdf_settings_validate';
  }
  else {
    $form['settings'] = array(
      '#type' => 'markup',
      '#value' => '<p>'. t("No PDF generation tool found! Please dowload a supported PHP PDF generation tool. Check this module's INSTALL.txt for more details.") .'</p>',
    );
  }

  return system_settings_form($form);
}

/**
 * Validate print_pdf_settings form.
 */
function _print_pdf_settings_validate($form, &$form_state) {
  if (empty($form_state['values']['print_pdf_pdf_tool'])) {
    form_set_error('print_pdf_pdf_tool', t("No PDF tool selected"));
  }
  if ($form_state['values']['print_pdf_font_size'] < 1) {
    form_set_error('print_pdf_font_size', t("Font size must be at least 1."));
  }
}

/**
 * Auxiliary function to locate suitable PDF generation tools
 *
 * @return
 *   array of filenames with the include-able PHP file of the located tools
 */
function _print_pdf_tools() {
  $tools = array_keys(file_scan_directory(drupal_get_path('module', 'print'), '^dompdf_config.inc.php$'));
  $tools = array_merge($tools, array_keys(file_scan_directory(drupal_get_path('module', 'print'), '^tcpdf.php$')));
  $tools = array_merge($tools, array_keys(file_scan_directory(drupal_get_path('module', 'print'), '^wkhtmltopdf')));
  $tools = array_merge($tools, array_keys(file_scan_directory(PRINT_PDF_LIB_PATH, '^dompdf_config.inc.php$')));
  $tools = array_merge($tools, array_keys(file_scan_directory(PRINT_PDF_LIB_PATH, '^tcpdf.php$')));
  $tools = array_merge($tools, array_keys(file_scan_directory(PRINT_PDF_LIB_PATH, '^wkhtmltopdf')));

  $num_tools = count($tools);

  if ($num_tools == 0) {
    variable_set('print_pdf_pdf_tool', PRINT_PDF_PDF_TOOL_DEFAULT);
    return -1;
  }
  else {
    // Instead of array_combine(), use this to maintain PHP4 compatibility
    $tools2 = array();
    foreach ($tools as $key => $val) {
      $tools2[$val] = $val;
    }

    return $tools2;
  }
}

/**
 * Menu callback for the PDF version module text strings settings form.
 *
 * @ingroup forms
 */
function print_pdf_strings_settings() {
  drupal_set_message(t("Saving these strings will disable their translation via Drupal's language system. Use the reset button to return them to the original state."), 'warning');

  $form['print_pdf_text'] = array(
    '#type' => 'fieldset',
    '#title' => t('Text strings'),
  );

  $form['print_pdf_text']['print_pdf_link_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Link text'),
    '#default_value' => variable_get('print_pdf_link_text', t('PDF version')),
    '#description' => t('Text used in the link to the PDF version.'),
  );

  return system_settings_form($form);
}
