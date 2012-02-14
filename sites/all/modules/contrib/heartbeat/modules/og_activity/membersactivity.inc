<?php
// $Id $

/**
 * @file
 *   HeartbeatAccess stream object for organic group activity
 */


heartbeat_include('HeartbeatAccess');

/**
 * Class MembersActivity
 * Concrete class to prepare messages for the current user
 * and all of his/her relations.
 *
 */
class MembersActivity extends HeartbeatAccess {

  protected $_gid = 0;
  protected $_group_access = FALSE;
  protected $group = NULL;
  
  public function construct() {

    // Where the user id is a member of the group
    // and where the nid target is the group nid
    $this->setGroup();
    drupal_add_js(drupal_get_path('module', 'og_activity') . '/og_activity.js');
    drupal_add_js(array('group_nid' => $this->_gid), "setting");
        
  }

  protected function hasAccess() {

    // if the user has access to this node (member or public group)
    //og_set_group_context($this->group);
    if (isset($this->group) && (og_is_group_member($this->group) || $this->group->og_private == 0)) {
      $this->_group_access = TRUE;
    }
    
    if (!isset($this->group)) {
    	$this->setError('We are not in a group context.');
    }

    return $this->_group_access;
    
  }

  private function setGroup($nid = 0) {

    if ($nid != 0) {
      $this->_gid = $nid;
      $this->group = node_load($nid);
      return;
    }

    if (!empty($_REQUEST['group_nid'])) {
      $this->_gid = $_REQUEST['group_nid'];
      $this->group = node_load($this->_gid);
    }
    elseif (arg(0) == 'node' && is_numeric(arg(1))) {

      $node = node_load(arg(1));

      if (og_is_group_type($node->type)) {
        $this->_gid = arg(1);
        $this->group = $node;
      }
      elseif (og_is_group_post_type($node->type)) {
        $this->_gid = current($node->og_groups);
        $this->group = node_load($this->_gid);
      }
    }

  }

  public function dressUpMessages(HeartbeatParser $heartbeat) {

    $members = array();
    $sql = og_list_users_sql(1, 0, NULL);
    $res = db_query($sql, $this->_gid);
    while ($row = db_fetch_object($res)) {
      $members[] = $row->uid;
    }

    $sql = " AND ua.uid IN (". implode(',', $members) .") ";
    $heartbeat->raw_messages = $this->resultSql($sql);

    return $heartbeat;
  }

  /**
   * Function to add a part of a sql to a query built by views UI
   *
   * @param object $view The view handler object by reference to add our part to the query
   */
  public function addViewQuery(&$view) {


    $members = array();
    $sql = og_list_users_sql(1, 0, NULL);
    $res = db_query($sql, $this->_gid);
    while ($row = db_fetch_object($res)) {
      $members[] = $row->uid;
    }

    $sql = "$view->table_alias.uid IN (". implode(',', $members) .") ";

    $view->query->set_where_group('AND', 'extra');
    $view->query->add_where('extra', $sql);
  }
}