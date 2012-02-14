<?php

/**
 * A helper class that wraps around the actual views plugin.
 *
 * @see views_php_plugin_query
 * @see views_php_plugin_pager
 */
class views_php_plugin_wrapper {

  protected $wrapped;
  protected $wrapped_link;

  public function php_wrap(&$link) {
    $this->wrapped_link = &$link;
    $this->wrapped = $link;
    $link = $this;
  }

  public function php_unwrap() {
    $this->wrapped_link = $this->wrapped;
    unset($this->wrapped);
    unset($this->wrapped_link);
  }

  public function &__get($name) {
    return $this->wrapped->$name;
  }

  public function __set($name, $value) {
    return $this->wrapped->$name = $value;
  }

  public function __isset($name) {
    return isset($this->wrapped->$name);
  }

  public function __unset($name) {
    unset($this->wrapped[$name]);
  }

  public function __call($name, $arguments) {
    return call_user_func_array(array($this->wrapped, $name), $arguments);
  }

  /**  As of PHP 5.3.0  */
  public static function __callStatic($name, $arguments) {
    return call_user_func_array(array(get_class($this->wrapped), $name), $arguments);
  }
}
