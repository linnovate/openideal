<?php
// $Id: schema_mysqli.inc,v 1.2 2009/06/06 15:32:02 mikeryan Exp $

function schema_mysqli_engine_type_map() {
  return schema_mysql_engine_type_map();
}

function schema_mysqli_schema_type_map() {
  return schema_mysql_schema_type_map();
}

function schema_mysqli_create_table_sql($table) {
  return schema_mysql_create_table_sql($table);
}

function schema_mysqli_inspect($name = NULL) {
  return schema_mysql_inspect($name);
}
