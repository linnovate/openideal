<?php

include "test_inc.php";

class IssuesTest extends ScssUnitTest {

    public function test_interpolator01(){
        $source = <<<END
@mixin test(\$testl) {
  #{\$testl} {
    width: 900px;
  }
}
@include test("body");
END;
        $expected = <<<END
body {
  width: 900px;
}
END;
        $this->assertScss($source, $expected);
    }

}


$unit = new IssuesTest();
$unit->run();
