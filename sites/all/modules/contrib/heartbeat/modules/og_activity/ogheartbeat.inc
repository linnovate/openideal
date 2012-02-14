<?php
// $Id: ogheartbeat.inc,v 1.1.2.7 2010/08/16 21:56:59 stalski Exp $

/**
 * @file
 *   HeartbeatAccess stream object for organic group activity
 */


heartbeat_include('HeartbeatAccess');

/**
 * Class OgHeartbeat
 * Concrete class to prepare messages for the current user
 * and all of his/her relations.
 *
 */
class OgHeartbeat extends HeartbeatAccess {

  protected $_gid = 0;
  protected $_group_access = FALSE;
  protected $group = NULL;
  
  public function construct() {

    // Where the user id is a member of the group
    // and where the nid target is the group nid
    $this->setGroup();
    
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

    if (arg(0) == 'node' && is_numeric(arg(1))) {

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

    $sql = " AND ua.nid <> 0 AND (ua.nid = %d OR ua.nid_target = %d) ";

    $heartbeat->raw_messages = $this->resultSql($sql, array($this->_gid, $this->_gid));

    return $heartbeat;
  }

  /**
   * Function to add a part of a sql to a query built by views UI
   *
   * @param object $view The view handler object by reference to add our part to the query
   */
  public function addViewQuery(&$view) {

    // Make the sql limited to the access
    $field = "$view->table_alias.$view->real_field";

    $view->query->set_where_group('AND', 'extra');

    $sql = "$view->table_alias.nid <> 0 AND ( $view->table_alias.nid = %d OR $view->table_alias.nid_target = %d ) ";
    $view->query->add_where('extra', $sql, $this->_gid, $this->_gid);
  }
}