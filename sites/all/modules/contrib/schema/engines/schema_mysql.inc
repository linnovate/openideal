<?php
// $Id: schema_mysql.inc,v 1.23.2.4 2010/02/01 19:03:19 mikeryan Exp $

function schema_mysql_engine_type_map() {
  $map = db_type_map();
  return $map;
}

function schema_mysql_schema_type_map() {
  static $map;
  if (!isset($map)) {
    $map = array_flip(array_map('strtolower', schema_mysql_engine_type_map()));
  }
  return $map;
}

function schema_mysql_create_table_sql($table) {
  $sql_cols = array();
  foreach ($table['fields'] as $colname => $col) {
    $sql = $colname.' '.schema_engine_type($col, 'mysql');
    if (isset($col['unsigned']) && $col['unsigned']) {
      $sql .= ' UNSIGNED';
    }
    unset($col['unsigned']);
    if (isset($col['length']) && $col['length']) {
      $sql .= '('.$col['length'].')';
    }
    if (isset($col['type']) && $col['type'] == 'serial') {
      $sql .= ' AUTO_INCREMENT';
    }
    unset($col['type']);
    unset($col['length']);

    if (isset($col['not null']) && $col['not null']) {
      $sql .= ' NOT NULL';
    }
    unset($col['not null']);
    foreach ($col as $prop => $val) {
      switch ($prop) {
        case 'default':
          $sql .= " $prop ";
          if (is_string($val)) {
            $sql .= "'$val'";
          } else {
            $sql .= $val;
          }
          break;
      }
    }
    $sql_cols[] = $sql;
  }

  $sql_keys = array();
  if (is_array($table['primary key'])) {
    $sql_keys[] = 'PRIMARY KEY ('.implode(', ', $table['primary key']).')';
  }
  foreach (array('unique keys', 'indexes') as $type) {
    if (isset($table[$type]) && is_array($table[$type])) {
      foreach ($table[$type] as $keyname => $key) {
        $sql = '';
        if ($type == 'unique keys') {
            $sql = 'UNIQUE ';
        }
        $sql .= 'KEY '.$keyname.' ';
        $sql .= '('.implode(', ', $key).')';
        $sql_keys[] = $sql;
      }
    }
  }

  $sql = "CREATE TABLE {".$table['name']."} (\n\t";
  $sql .= implode(",\n\t", $sql_cols);
  if (count($sql_keys) > 0) {
    $sql .= ",\n\t";
  }
  $sql .= implode(",\n\t", $sql_keys);
  $sql .= "\n";
  $sql .= ") /*!40100 DEFAULT CHARACTER SET utf8 */;\n\n";
  return $sql;
}

function schema_mysql_inspect($name = NULL) {
  global $db_url;

  // Switch to the active database connection.
  // The only way to get the active connection's name is as a return value from
  // db_set_active(). However, calling this function will automatically switch
  // the active connection to 'default', which might not be what we want.
  // Therefore, we must immediately call db_set_active() again with the desired
  // connection name in order to proceed.
  $active_db_connection = db_set_active();
  db_set_active($active_db_connection);

  $tables = array();
  $url = parse_url(is_array($db_url) ? $db_url[$active_db_connection] : $db_url);
  $database = substr($url['path'], 1);

  $sql = 'SELECT TABLE_NAME, TABLE_COMMENT
          FROM information_schema.TABLES
          WHERE TABLE_SCHEMA="%s" ';
  if (isset($name)) {
    $sql .= 'AND TABLE_NAME = "%s" ';
  }
  $res = db_query($sql, $database, $name);
  while ($r = db_fetch_array($res)) {
    $tables[$r['TABLE_NAME']]['description'] = $r['TABLE_COMMENT'];
  }

  $sql = 'SELECT TABLE_NAME, COLUMN_TYPE, COLUMN_NAME, COLUMN_DEFAULT,
                 EXTRA, IS_NULLABLE, NUMERIC_SCALE, COLUMN_COMMENT
          FROM information_schema.COLUMNS
          WHERE TABLE_SCHEMA="%s" ';
  if (isset($name)) {
    $sql .= 'AND TABLE_NAME = "%s" ';
  }
  $sql .= 'ORDER BY TABLE_NAME, ORDINAL_POSITION';

  $res = db_query($sql, $database, $name);
  while ($r = db_fetch_array($res)) {
    $r['NEW_TABLE_NAME'] = schema_unprefix_table($r['TABLE_NAME']);

    $numeric = !is_null($r['NUMERIC_SCALE']);
    $col = array();
    $col['type'] = $r['COLUMN_TYPE'];
    if (preg_match('@([a-z]+)(?:\((\d+)(?:,(\d+))?\))?\s*(unsigned)?@', $col['type'], $matches)) {
      list($col['type'], $col['size']) = schema_schema_type($matches[1], $r['TABLE_NAME'], $r['COLUMN_NAME'], 'mysql');
      if (isset($matches[2])) {
        if ($col['type'] == 'numeric' || $col['type'] == 'float' || $col['type'] == 'double') {
          $col['precision'] = $matches[2];
          $col['scale'] = $matches[3];
        }
        else if (!$numeric) {
          $col['length'] = $matches[2];
        }
      }
      if (isset($matches[4])) {
        $col['unsigned'] = TRUE;
      }
    }
    if ($col['type'] == 'int' && isset($r['EXTRA']) &&
      $r['EXTRA'] == 'auto_increment') {
      $col['type'] = 'serial';
    }
    $col['not null'] = ($r['IS_NULLABLE'] == 'YES' ? FALSE : TRUE);
    if (! is_null($r['COLUMN_DEFAULT'])) {
      if ($numeric) {
        // XXX floats!
        $col['default'] = intval($r['COLUMN_DEFAULT']);
      } else {
        $col['default'] = $r['COLUMN_DEFAULT'];
      }
    }
    $col['description'] = $r['COLUMN_COMMENT'];
    $tables[$r['TABLE_NAME']]['fields'][$r['COLUMN_NAME']] = $col;
    // At this point, $tables is indexed by the raw db table name - save the unprefixed
    // name for later use
    $tables[$r['TABLE_NAME']]['name'] = $r['NEW_TABLE_NAME'];
  }

  $sql = 'SELECT TABLE_NAME, COLUMN_NAME, INDEX_NAME, SUB_PART, NON_UNIQUE
          FROM information_schema.STATISTICS
          WHERE TABLE_SCHEMA="%s" ';
  if (isset($name)) {
    $sql .= 'AND TABLE_NAME = "%s" ';
  }
  $sql .= 'ORDER BY TABLE_NAME, INDEX_NAME, SEQ_IN_INDEX';

  $res = db_query($sql, $database, $name);
  while ($r = db_fetch_array($res)) {
    if (isset($r['SUB_PART']) && !is_null($r['SUB_PART'])) {
      $col = array($r['COLUMN_NAME'], intval($r['SUB_PART']));
    } else {
      $col = $r['COLUMN_NAME'];
    }
    if ($r['INDEX_NAME'] == 'PRIMARY') {
      $type = 'primary key';
      $tables[$r['TABLE_NAME']][$type][] = $col;
      continue;
    } else if ($r['NON_UNIQUE'] == 0) {
      $type = 'unique keys';
    } else {
      $type = 'indexes';
    }
    $tables[$r['TABLE_NAME']][$type][$r['INDEX_NAME']][] = $col;
  }

  // Now, for tables which we have unprefixed, index $tables by the unprefixed name
  foreach ($tables as $tablename => $table) {
    $newname = $tables[$tablename]['name'];
    if ($tablename != $newname) {
      $tables[$newname] = $table;
      unset($tables[$tablename]);
    }
  }

  return $tables;
}
