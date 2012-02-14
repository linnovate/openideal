<?php

/**
 * @file
 * Miscellaneous functions for the xmlsitemap module.
 *
 * @ingroup xmlsitemap
 */

/**
 * Fetch a short blurb string about module maintainership and sponsors.
 *
 * This message will be FALSE in 'official' releases.
 */
function _xmlsitemap_get_blurb($check_version = TRUE) {
  static $blurb;

  if (!isset($blurb)) {
    $blurb = FALSE;
    if (!$check_version || (($version = _xmlsitemap_get_version()) && preg_match('/dev|unstable|alpha|beta|HEAD/i', $version))) {
      $sponsors = array(
        l('Symantec', 'http://www.symantec.com/'),
        l('WebWise Solutions', 'http://www.webwiseone.com/'),
        l('Volacci', 'http://www.volacci.com/'),
        l('lanetro', 'http://www.lanetro.com/'),
        l('Coupons Dealuxe', 'http://couponsdealuxe.com/'),
      );
      // Don't extract the following string for translation.
      $blurb = '<div class="description"><p>Thank you for helping test the XML sitemap module rewrite. Please consider helping offset developer free time by <a href="http://davereid.chipin.com/">donating</a> or if your company is interested in sponsoring the rewrite or a specific feature, please <a href="http://davereid.net/contact">contact the developer</a>. Thank you to the following current sponsors: ' . implode(', ', $sponsors) . ', and all the individuals that have donated. This message will not be seen in the stable versions.</p></div>';
      //http://drupalmodules.com/module/xml-sitemap
    }
  }

  return $blurb;
}

function _xmlsitemap_get_version() {
  static $version;
  if (!isset($version)) {
    $modules = _system_rebuild_module_data();
    $version = $modules['xmlsitemap']->info['version'];
  }
  return $version;
}

/**
 * Check the status of all hook_requirements() from any xmlsitemap modules.
 */
function xmlsitemap_check_status() {
  $messages = &drupal_static(__FUNCTION__);

  if (!isset($messages)) {
    // Cache the list of modules that are checked.
    if ($cache = cache_get('xmlsitemap:registry:requirements')) {
      $modules = $cache->data;
    }
    else {
      $modules = array();
      module_load_all_includes('install');
      foreach (module_implements('requirements') as $module) {
        if (strpos($module, 'xmlsitemap') !== FALSE) {
          $modules[] = $module;
        }
      }
      cache_set('xmlsitemap:registry:requirements', $modules);
    }

    $messages = array();
    foreach ($modules as $module) {
      module_load_install($module);
      $requirements = module_invoke($module, 'requirements', 'runtime');
      foreach ($requirements as $requirement) {
        if (isset($requirement['severity']) && max(REQUIREMENT_OK, $requirement['severity'])) {
          $messages[] = $requirement['description'];
        }
      }
    }

    if ($messages) {
      $message = t('One or more problems were detected with your XML sitemap configuration: !messages', array('!messages' => theme('item_list', array('items' => $messages))));
      if (user_access('access site reports')) {
        $message .= t('Check the <a href="@status-report">status report</a> for more information.', array('@status-report' => url('admin/reports/status')));
      }
      drupal_set_message($message, 'warning', FALSE);
    }
  }

  return !empty($messages);
}
