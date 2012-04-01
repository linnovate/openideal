<?php

include "test_inc.php";

class IssuesTest extends ScssUnitTest {

    public function test_ifs(){
        $tests = array(
            '{ @if #ccc {color: #111; } }' => 'color: #111111;',
            '{ @if 123 {color: #222; } }' => 'color: #222222;',
            '{ @if true {color: #333; } }' => 'color: #333333;',
            '{ @if "test" {color: #444; } }' => 'color: #444444;',
            '{ @if 0 {color: #555; } }' => '',
            '{ @if 0 {color: #666; } @else {color: #777; } }' => 'color: #777777;',
        );
        $this->assertScss(array_keys($tests), array_values($tests));
    }

}


$unit = new IssuesTest();
$unit->run();
