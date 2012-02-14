<?php

/**
 * Implements hook_ctools_plugin_api().
 */
function fe_main_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "boxes" && $api == "box") {
    return array("version" => 1);
  }
  elseif ($module == "context" && $api == "context") {
    return array("version" => 3);
  }
  elseif ($module == "quicktabs" && $api == "quicktabs") {
    return array("version" => 1);
  }
  elseif ($module == "strongarm" && $api == "strongarm") {
    return array("version" => 1);
  }
}

/**
 * Implements hook_flag_default_flags().
 */
function fe_main_flag_default_flags() {
  $flags = array();
  // Exported flag: "In Action".
  $flags[] = array(
    'content_type' => 'node',
    'name' => 'in_action',
    'title' => 'In Action',
    'global' => '1',
    'types' => array(
      0 => 'project',
    ),
    'flag_short' => 'Flag as In Action ',
    'flag_long' => 'close this idea for comments and voting',
    'flag_message' => '',
    'unflag_short' => 'Unflagged as In Action ',
    'unflag_long' => 'open this idea for comments and voting',
    'unflag_message' => '',
    'unflag_denied_text' => '',
    'link_type' => 'normal',
    'roles' => array(
      'flag' => array(
        0 => 3,
        1 => 7,
      ),
      'unflag' => array(
        0 => 3,
        1 => 7,
      ),
    ),
    'show_on_page' => 0,
    'show_on_teaser' => 0,
    'show_on_form' => 0,
    'access_author' => '',
    'i18n' => 0,
    'module' => 'fe_main',
    'api_version' => 2,
  );
  // Exported flag: "Popular".
  $flags[] = array(
    'content_type' => 'node',
    'name' => 'popular',
    'title' => 'Popular',
    'global' => '1',
    'types' => array(
      0 => 'project',
    ),
    'flag_short' => 'flag as popular',
    'flag_long' => '',
    'flag_message' => '',
    'unflag_short' => 'unflag as popular',
    'unflag_long' => '',
    'unflag_message' => '',
    'unflag_denied_text' => '',
    'link_type' => 'toggle',
    'roles' => array(
      'flag' => array(
        0 => 3,
        1 => 7,
      ),
      'unflag' => array(
        0 => 3,
        1 => 7,
      ),
    ),
    'show_on_page' => 0,
    'show_on_teaser' => 0,
    'show_on_form' => 0,
    'access_author' => '',
    'i18n' => 0,
    'module' => 'fe_main',
    'api_version' => 2,
  );
  // Exported flag: "Worth checking".
  $flags[] = array(
    'content_type' => 'node',
    'name' => 'worth_checking',
    'title' => 'Worth checking',
    'global' => '1',
    'types' => array(
      0 => 'project',
    ),
    'flag_short' => 'flag as worth checking',
    'flag_long' => '',
    'flag_message' => '',
    'unflag_short' => 'unflagged worth checking',
    'unflag_long' => '',
    'unflag_message' => '',
    'unflag_denied_text' => '',
    'link_type' => 'toggle',
    'roles' => array(
      'flag' => array(
        0 => 3,
        1 => 7,
      ),
      'unflag' => array(
        0 => 3,
        1 => 7,
      ),
    ),
    'show_on_page' => 1,
    'show_on_teaser' => 1,
    'show_on_form' => 0,
    'access_author' => '',
    'i18n' => 0,
    'module' => 'fe_main',
    'api_version' => 2,
  );
  return $flags;
}

/**
 * Implements hook_imagecache_default_presets().
 */
function fe_main_imagecache_default_presets() {
  $items = array(
    '140x140_thumb' => array(
      'presetname' => '140x140_thumb',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale_and_crop',
          'data' => array(
            'width' => '140',
            'height' => '140',
          ),
        ),
      ),
    ),
    '287px-188px' => array(
      'presetname' => '287px-188px',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale',
          'data' => array(
            'width' => '287',
            'height' => '188',
            'upscale' => 0,
          ),
        ),
      ),
    ),
    '30x30_thumb' => array(
      'presetname' => '30x30_thumb',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale',
          'data' => array(
            'width' => '30',
            'height' => '30',
            'upscale' => 0,
          ),
        ),
      ),
    ),
    '45x45_thumb' => array(
      'presetname' => '45x45_thumb',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale',
          'data' => array(
            'width' => '45',
            'height' => '45',
            'upscale' => 0,
          ),
        ),
      ),
    ),
    '80x80_thumb' => array(
      'presetname' => '80x80_thumb',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale_and_crop',
          'data' => array(
            'width' => '80',
            'height' => '80',
          ),
        ),
      ),
    ),
  );
  return $items;
}

/**
 * Implements hook_node_info().
 */
function fe_main_node_info() {
  $items = array(
    'idea' => array(
      'name' => t('Idea'),
      'base' => 'features',
      'description' => t('An idea which suggested by a user'),
      'has_title' => '1',
      'title_label' => t('idea'),
      'has_body' => '1',
      'body_label' => t('description'),
      'min_word_count' => '0',
      'help' => '',
    ),
    'profile' => array(
      'name' => t('user profile'),
      'base' => 'features',
      'description' => t('A user profile built as content.'),
      'has_title' => '1',
      'title_label' => t('title'),
      'has_body' => '0',
      'body_label' => '',
      'min_word_count' => '0',
      'help' => '',
    ),
  );
  return $items;
}

/**
 * Implements hook_rules_defaults().
 */
function fe_main_rules_defaults() {
  return array(
    'rules' => array(
      'fe_main_86' => array(
        '#type' => 'rule',
        '#set' => 'event_node_insert',
        '#label' => 'Idea points',
        '#active' => 1,
        '#weight' => '0',
        '#categories' => array(
          '0' => 'points',
          'fe_main' => 'fe_main',
        ),
        '#status' => 'default',
        '#conditions' => array(
          '0' => array(
            '#weight' => 0,
            '#type' => 'condition',
            '#settings' => array(
              'type' => array(
                'idea' => 'idea',
              ),
              '#argument map' => array(
                'node' => 'node',
              ),
            ),
            '#name' => 'rules_condition_content_is_type',
            '#info' => array(
              'label' => 'Created content is Idea',
              'arguments' => array(
                'node' => array(
                  'type' => 'node',
                  'label' => 'Content',
                ),
              ),
              'module' => 'Node',
            ),
          ),
          '1' => array(
            '#negate' => 1,
            '#weight' => 0,
            '#info' => array(
              'label' => 'User has role(s)',
              'arguments' => array(
                'user' => array(
                  'type' => 'user',
                  'label' => 'user',
                ),
              ),
              'module' => 'User',
            ),
            '#name' => 'rules_condition_user_hasrole',
            '#settings' => array(
              'roles' => array(
                '0' => 3,
                '1' => 4,
                '2' => 7,
              ),
              'operation' => 'OR',
              '#argument map' => array(
                'user' => 'user',
              ),
            ),
            '#type' => 'condition',
          ),
        ),
        '#actions' => array(
          '0' => array(
            '#info' => array(
              'label' => 'Grant points to a user',
              'arguments' => array(
                'user' => array(
                  'type' => 'user',
                  'label' => 'User',
                ),
              ),
              'module' => 'Userpoints',
              'eval input' => array(
                '0' => 'points',
              ),
            ),
            '#name' => 'userpoints_action_grant_points',
            '#settings' => array(
              'points' => '5',
              '#argument map' => array(
                'user' => 'author',
              ),
            ),
            '#type' => 'action',
            '#weight' => 0,
          ),
        ),
        '#version' => 6003,
      ),
      'fe_main_87' => array(
        '#type' => 'rule',
        '#set' => 'event_comment_insert',
        '#label' => 'Comment points',
        '#active' => 1,
        '#weight' => '0',
        '#categories' => array(
          '0' => 'points',
          'fe_main' => 'fe_main',
        ),
        '#status' => 'default',
        '#conditions' => array(
          '0' => array(
            '#type' => 'condition',
            '#settings' => array(
              'type' => array(
                'idea' => 'idea',
              ),
              '#argument map' => array(
                'node' => 'node',
              ),
            ),
            '#name' => 'rules_condition_content_is_type',
            '#info' => array(
              'label' => 'Commented content is Idea',
              'arguments' => array(
                'node' => array(
                  'type' => 'node',
                  'label' => 'Content',
                ),
              ),
              'module' => 'Node',
            ),
            '#weight' => 0,
          ),
          '1' => array(
            '#name' => 'rules_condition_user_hasrole',
            '#info' => array(
              'label' => 'User has role(s)',
              'arguments' => array(
                'user' => array(
                  'type' => 'user',
                  'label' => 'משתמש',
                ),
              ),
              'module' => 'User',
            ),
            '#settings' => array(
              'roles' => array(
                '0' => 3,
                '1' => 4,
                '2' => 7,
              ),
              'operation' => 'OR',
              '#argument map' => array(
                'user' => 'user',
              ),
            ),
            '#type' => 'condition',
            '#weight' => 0,
            '#negate' => 1,
          ),
        ),
        '#actions' => array(
          '0' => array(
            '#weight' => 0,
            '#info' => array(
              'label' => 'Grant points to a user',
              'arguments' => array(
                'user' => array(
                  'type' => 'user',
                  'label' => 'User',
                ),
              ),
              'module' => 'Userpoints',
              'eval input' => array(
                '0' => 'points',
              ),
            ),
            '#name' => 'userpoints_action_grant_points',
            '#settings' => array(
              'points' => '3',
              '#argument map' => array(
                'user' => 'comment_author',
              ),
            ),
            '#type' => 'action',
          ),
        ),
        '#version' => 6003,
      ),
      'fe_main_88' => array(
        '#type' => 'rule',
        '#set' => 'event_node_presave',
        '#label' => 'In action - under review',
        '#active' => 1,
        '#weight' => '0',
        '#categories' => array(
          '0' => 'points',
          'fe_main' => 'fe_main',
        ),
        '#status' => 'default',
        '#conditions' => array(
          '0' => array(
            '#weight' => 0,
            '#info' => array(
              'label' => 'Saved content\'s field \'field_in_action\' has been changed',
              'arguments' => array(
                'node' => array(
                  'type' => 'node',
                  'label' => 'Content containing changes',
                ),
                'node_unchanged' => array(
                  'type' => 'node',
                  'label' => 'Content not containing changes',
                ),
              ),
              'module' => 'CCK',
            ),
            '#name' => 'content_rules_field_changed',
            '#settings' => array(
              'field_name' => 'field_in_action',
              '#argument map' => array(
                'node' => 'node',
                'node_unchanged' => 'node_unchanged',
              ),
            ),
            '#type' => 'condition',
          ),
          '1' => array(
            '#type' => 'condition',
            '#settings' => array(
              'field_name' => 'field_in_action',
              '#argument map' => array(
                'node' => 'node',
              ),
              'value' => array(
                '0' => array(
                  'value' => 'under_review',
                ),
              ),
              'code' => '',
              'vars' => array(),
            ),
            '#name' => 'content_rules_field_has_value',
            '#info' => array(
              'label' => 'Saved content\'s field \'field_in_action\' has value',
              'arguments' => array(
                'node' => array(
                  'type' => 'node',
                  'label' => 'Content',
                ),
              ),
              'eval input' => array(
                '0' => 'code',
                '1' => 'value|0|value',
              ),
              'module' => 'CCK',
            ),
            '#weight' => 0,
          ),
          '2' => array(
            '#negate' => 1,
            '#weight' => 0,
            '#info' => array(
              'label' => 'User has role(s)',
              'arguments' => array(
                'user' => array(
                  'type' => 'user',
                  'label' => 'user',
                ),
              ),
              'module' => 'User',
            ),
            '#name' => 'rules_condition_user_hasrole',
            '#settings' => array(
              'roles' => array(
                '0' => 3,
                '1' => 4,
                '2' => 7,
              ),
              'operation' => 'OR',
              '#argument map' => array(
                'user' => 'user',
              ),
            ),
            '#type' => 'condition',
          ),
        ),
        '#actions' => array(
          '1' => array(
            '#info' => array(
              'label' => 'Grant points to a user',
              'arguments' => array(
                'user' => array(
                  'type' => 'user',
                  'label' => 'User',
                ),
              ),
              'module' => 'Userpoints',
              'eval input' => array(
                '0' => 'points',
              ),
            ),
            '#name' => 'userpoints_action_grant_points',
            '#settings' => array(
              'points' => '3',
              '#argument map' => array(
                'user' => 'author',
              ),
            ),
            '#type' => 'action',
            '#weight' => 0,
          ),
        ),
        '#version' => 6003,
      ),
      'fe_main_89' => array(
        '#type' => 'rule',
        '#set' => 'event_node_presave',
        '#label' => 'In action - launched',
        '#active' => 1,
        '#weight' => '0',
        '#categories' => array(
          '0' => 'points',
          'fe_main' => 'fe_main',
        ),
        '#status' => 'default',
        '#conditions' => array(
          '0' => array(
            '#weight' => 0,
            '#info' => array(
              'label' => 'Saved content\'s field \'field_in_action\' has been changed',
              'arguments' => array(
                'node' => array(
                  'type' => 'node',
                  'label' => 'Content containing changes',
                ),
                'node_unchanged' => array(
                  'type' => 'node',
                  'label' => 'Content not containing changes',
                ),
              ),
              'module' => 'CCK',
            ),
            '#name' => 'content_rules_field_changed',
            '#settings' => array(
              'field_name' => 'field_in_action',
              '#argument map' => array(
                'node' => 'node',
                'node_unchanged' => 'node_unchanged',
              ),
            ),
            '#type' => 'condition',
          ),
          '1' => array(
            '#type' => 'condition',
            '#settings' => array(
              'field_name' => 'field_in_action',
              '#argument map' => array(
                'node' => 'node',
              ),
              'value' => array(
                '0' => array(
                  'value' => 'launched',
                ),
              ),
              'code' => '',
              'vars' => array(),
            ),
            '#name' => 'content_rules_field_has_value',
            '#info' => array(
              'label' => 'Saved content\'s field \'field_in_action\' has value',
              'arguments' => array(
                'node' => array(
                  'type' => 'node',
                  'label' => 'Content',
                ),
              ),
              'eval input' => array(
                '0' => 'code',
                '1' => 'value|0|value',
              ),
              'module' => 'CCK',
            ),
            '#weight' => 0,
          ),
          '2' => array(
            '#negate' => 1,
            '#weight' => 0,
            '#info' => array(
              'label' => 'User has role(s)',
              'arguments' => array(
                'user' => array(
                  'type' => 'user',
                  'label' => 'משתמש',
                ),
              ),
              'module' => 'User',
            ),
            '#name' => 'rules_condition_user_hasrole',
            '#settings' => array(
              'roles' => array(
                '0' => 3,
                '1' => 4,
                '2' => 7,
              ),
              'operation' => 'OR',
              '#argument map' => array(
                'user' => 'user',
              ),
            ),
            '#type' => 'condition',
          ),
        ),
        '#actions' => array(
          '1' => array(
            '#info' => array(
              'label' => 'Grant points to a user',
              'arguments' => array(
                'user' => array(
                  'type' => 'user',
                  'label' => 'User',
                ),
              ),
              'module' => 'Userpoints',
              'eval input' => array(
                '0' => 'points',
              ),
            ),
            '#name' => 'userpoints_action_grant_points',
            '#settings' => array(
              'points' => '10',
              '#argument map' => array(
                'user' => 'author',
              ),
            ),
            '#type' => 'action',
            '#weight' => 0,
          ),
        ),
        '#version' => 6003,
      ),
      'fe_main_91' => array(
        '#type' => 'rule',
        '#set' => 'rules_points_to_comments_authors',
        '#label' => 'under review comment author points',
        '#active' => 1,
        '#weight' => '0',
        '#categories' => array(
          '0' => 'points',
          '1' => 'fe_main',
        ),
        '#status' => 'default',
        '#conditions' => array(
          '0' => array(
            '#info' => array(
              'label' => 'Content\'s field \'field_in_action\' has been changed',
              'arguments' => array(
                'node' => array(
                  'type' => 'node',
                  'label' => 'Content containing changes',
                ),
                'node_unchanged' => array(
                  'type' => 'node',
                  'label' => 'Content not containing changes',
                ),
              ),
              'module' => 'CCK',
            ),
            '#name' => 'content_rules_field_changed',
            '#settings' => array(
              'field_name' => 'field_in_action',
              '#argument map' => array(
                'node' => 'node',
                'node_unchanged' => 'node2',
              ),
            ),
            '#type' => 'condition',
            '#weight' => 0,
          ),
          '1' => array(
            '#weight' => 0,
            '#info' => array(
              'label' => 'Content\'s field \'field_in_action\' has value',
              'arguments' => array(
                'node' => array(
                  'type' => 'node',
                  'label' => 'Content',
                ),
              ),
              'eval input' => array(
                '0' => 'code',
                '1' => 'value|0|value',
              ),
              'module' => 'CCK',
            ),
            '#name' => 'content_rules_field_has_value',
            '#settings' => array(
              'field_name' => 'field_in_action',
              '#argument map' => array(
                'node' => 'node',
              ),
              'value' => array(
                '0' => array(
                  'value' => 'Under Review',
                ),
              ),
              'code' => '',
              'vars' => array(),
            ),
            '#type' => 'condition',
          ),
          '2' => array(
            '#negate' => 1,
            '#weight' => 0,
            '#info' => array(
              'label' => 'User has role(s)',
              'arguments' => array(
                'user' => array(
                  'type' => 'user',
                  'label' => 'user',
                ),
              ),
              'module' => 'User',
            ),
            '#name' => 'rules_condition_user_hasrole',
            '#settings' => array(
              'roles' => array(
                '0' => 3,
                '1' => 4,
                '2' => 7,
              ),
              'operation' => 'OR',
              '#argument map' => array(
                'user' => 'user',
              ),
            ),
            '#type' => 'condition',
          ),
        ),
        '#actions' => array(
          '0' => array(
            '#weight' => 0,
            '#info' => array(
              'label' => 'Grant points to a user',
              'arguments' => array(
                'user' => array(
                  'type' => 'user',
                  'label' => 'User',
                ),
              ),
              'module' => 'Userpoints',
              'eval input' => array(
                '0' => 'points',
              ),
            ),
            '#name' => 'userpoints_action_grant_points',
            '#settings' => array(
              'points' => '1',
              '#argument map' => array(
                'user' => 'user',
              ),
            ),
            '#type' => 'action',
          ),
        ),
        '#version' => 6003,
      ),
      'fe_main_92' => array(
        '#type' => 'rule',
        '#set' => 'rules_points_to_comments_authors',
        '#label' => 'launched comment author points',
        '#active' => 1,
        '#weight' => '0',
        '#categories' => array(
          '0' => 'points',
          '1' => 'fe_main',
        ),
        '#status' => 'default',
        '#conditions' => array(
          '0' => array(
            '#type' => 'condition',
            '#settings' => array(
              'field_name' => 'field_in_action',
              '#argument map' => array(
                'node' => 'node',
                'node_unchanged' => 'node2',
              ),
            ),
            '#name' => 'content_rules_field_changed',
            '#info' => array(
              'label' => 'Content\'s field \'field_in_action\' has been changed',
              'arguments' => array(
                'node' => array(
                  'type' => 'node',
                  'label' => 'Content containing changes',
                ),
                'node_unchanged' => array(
                  'type' => 'node',
                  'label' => 'Content not containing changes',
                ),
              ),
              'module' => 'CCK',
            ),
            '#weight' => 0,
          ),
          '1' => array(
            '#weight' => 0,
            '#info' => array(
              'label' => 'Content\'s field \'field_in_action\' has value',
              'arguments' => array(
                'node' => array(
                  'type' => 'node',
                  'label' => 'Content',
                ),
              ),
              'eval input' => array(
                '0' => 'code',
                '1' => 'value|0|value',
              ),
              'module' => 'CCK',
            ),
            '#name' => 'content_rules_field_has_value',
            '#settings' => array(
              'field_name' => 'field_in_action',
              '#argument map' => array(
                'node' => 'node',
              ),
              'value' => array(
                '0' => array(
                  'value' => 'Launched',
                ),
              ),
              'code' => '',
              'vars' => array(),
            ),
            '#type' => 'condition',
          ),
          '2' => array(
            '#negate' => 1,
            '#weight' => 0,
            '#info' => array(
              'label' => 'User has role(s)',
              'arguments' => array(
                'user' => array(
                  'type' => 'user',
                  'label' => 'user',
                ),
              ),
              'module' => 'User',
            ),
            '#name' => 'rules_condition_user_hasrole',
            '#settings' => array(
              'roles' => array(
                '0' => 3,
                '1' => 4,
                '2' => 7,
              ),
              'operation' => 'OR',
              '#argument map' => array(
                'user' => 'user',
              ),
            ),
            '#type' => 'condition',
          ),
        ),
        '#actions' => array(
          '0' => array(
            '#type' => 'action',
            '#settings' => array(
              'points' => '2',
              '#argument map' => array(
                'user' => 'user',
              ),
            ),
            '#name' => 'userpoints_action_grant_points',
            '#info' => array(
              'label' => 'Grant points to a user',
              'arguments' => array(
                'user' => array(
                  'type' => 'user',
                  'label' => 'User',
                ),
              ),
              'module' => 'Userpoints',
              'eval input' => array(
                '0' => 'points',
              ),
            ),
            '#weight' => 0,
          ),
        ),
        '#version' => 6003,
      ),
      'fe_main_1' => array(
        '#type' => 'rule',
        '#set' => 'event_node_update',
        '#label' => 'content is in action',
        '#active' => 1,
        '#weight' => '0',
        '#categories' => array(
          '0' => 'in action',
          '1' => 'fe_main',
        ),
        '#status' => 'default',
        '#conditions' => array(
          '0' => array(
            '#type' => 'condition',
            '#settings' => array(
              'field_name' => 'field_in_action',
              '#argument map' => array(
                'node' => 'node',
                'node_unchanged' => 'node_unchanged',
              ),
            ),
            '#name' => 'content_rules_field_changed',
            '#info' => array(
              'label' => 'Saved content\'s field \'field_in_action\' has been changed',
              'arguments' => array(
                'node' => array(
                  'type' => 'node',
                  'label' => 'Content containing changes',
                ),
                'node_unchanged' => array(
                  'type' => 'node',
                  'label' => 'Content not containing changes',
                ),
              ),
              'module' => 'CCK',
            ),
            '#weight' => 0,
          ),
        ),
        '#actions' => array(
          '0' => array(
            '#weight' => 0,
            '#info' => array(
              'label' => 'Flag saved content, under "In Action"',
              'base' => 'flag_rules_action_flag',
              'label callback' => 'flag_rules_action_flag_label',
              'arguments' => array(
                'flag' => array(
                  'type' => 'flag',
                  'label' => 'Flag',
                  'flag_type' => 'node',
                ),
                'object' => array(
                  'type' => 'node',
                  'label' => 'Flagged content',
                ),
                'flagging_user' => array(
                  'type' => 'user',
                  'label' => 'User on whose behalf to flag',
                  'description' => 'For non-global flags, this is the user on whose behalf to flag the object. In addition, if checked below, the access permissions to the flag are checked against this user.',
                ),
              ),
              'module' => 'Flag',
            ),
            '#name' => 'flag_rules_action_flag_node',
            '#settings' => array(
              'flag' => 'in_action',
              'permission_check' => 0,
              '#argument map' => array(
                'object' => 'node',
                'flagging_user' => 'author',
              ),
            ),
            '#type' => 'action',
          ),
        ),
        '#version' => 6003,
      ),
      'fe_main_93' => array(
        '#type' => 'rule',
        '#set' => 'event_flag_flagged_in_action',
        '#label' => 'In Action - close for new comments',
        '#active' => 1,
        '#weight' => '0',
        '#categories' => array(
          '0' => 'in action',
          'fe_main' => 'fe_main',
        ),
        '#status' => 'default',
        '#conditions' => array(
          '0' => array(
            '#type' => 'condition',
            '#settings' => array(
              'type' => array(
                'idea' => 'idea',
              ),
              '#argument map' => array(
                'node' => 'node',
              ),
            ),
            '#name' => 'rules_condition_content_is_type',
            '#info' => array(
              'label' => 'Flagged content is Idea',
              'arguments' => array(
                'node' => array(
                  'type' => 'node',
                  'label' => 'Content',
                ),
              ),
              'module' => 'Node',
            ),
            '#weight' => 0,
          ),
        ),
        '#actions' => array(
          '0' => array(
            '#weight' => 0,
            '#info' => array(
              'label' => 'close node from adding new comments',
              'module' => 'Ideal',
              'arguments' => array(
                'node' => array(
                  'type' => 'node',
                  'label' => 'Content',
                ),
              ),
            ),
            '#name' => 'ideal_action_close_comments',
            '#settings' => array(
              '#argument map' => array(
                'node' => 'node',
              ),
            ),
            '#type' => 'action',
          ),
        ),
        '#version' => 6003,
      ),
      'fe_main_2' => array(
        '#type' => 'rule',
        '#set' => 'event_voting_rules_insert_node',
        '#label' => 'vote points',
        '#active' => 1,
        '#weight' => '0',
        '#categories' => array(
          '0' => 'points',
          'fe_main' => 'fe_main',
        ),
        '#status' => 'default',
        '#conditions' => array(
          '0' => array(
            '#weight' => 0,
            '#info' => array(
              'label' => 'The node being voted on is Idea',
              'arguments' => array(
                'node' => array(
                  'type' => 'node',
                  'label' => 'Content',
                ),
              ),
              'module' => 'Node',
            ),
            '#name' => 'rules_condition_content_is_type',
            '#settings' => array(
              'type' => array(
                'idea' => 'idea',
              ),
              '#argument map' => array(
                'node' => 'node',
              ),
            ),
            '#type' => 'condition',
          ),
        ),
        '#actions' => array(
          '0' => array(
            '#type' => 'action',
            '#settings' => array(
              'code' => ' userpoints_userpointsapi(1); ',
              'vars' => array(),
            ),
            '#name' => 'rules_action_custom_php',
            '#info' => array(
              'label' => 'Execute custom PHP code',
              'module' => 'PHP',
              'eval input' => array(
                '0' => 'code',
              ),
            ),
            '#weight' => 0,
          ),
        ),
        '#version' => 6003,
      ),
    ),
    'rule_sets' => array(
      'rules_points_to_comments_authors' => array(
        'arguments' => array(
          'user' => array(
            'label' => 'User',
            'type' => 'user',
          ),
          'node' => array(
            'label' => 'Content',
            'type' => 'node',
          ),
          'node2' => array(
            'label' => 'Content2',
            'type' => 'node',
          ),
        ),
        'label' => 'points to comments authors',
        'status' => 'default',
        'categories' => array(
          '0' => 'points',
          'fe_main' => 'fe_main',
        ),
      ),
    ),
  );
}

/**
 * Implements hook_views_api().
 */
function fe_main_views_api() {
  return array(
    'api' => '2',
  );
}
