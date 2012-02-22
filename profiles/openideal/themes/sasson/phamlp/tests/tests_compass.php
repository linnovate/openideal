<?php

include "test_inc.php";

class IssuesTest extends ScssUnitTest {

    public function test_border_radius(){
        $source = <<<END
@import "compass/css3/border-radius";
.box {
  @include border-radius(4px, 4px);
}
END;
        $expected = <<<END
.box {
  -webkit-border-radius: 4px 4px;
  -moz-border-radius: 4px / 4px;
  -o-border-radius: 4px / 4px;
  -ms-border-radius: 4px / 4px;
  -khtml-border-radius: 4px / 4px;
  border-radius: 4px / 4px;
}
END;
        $this->assertScss($source, $expected);
    }

}


$unit = new IssuesTest();
$unit->run();
