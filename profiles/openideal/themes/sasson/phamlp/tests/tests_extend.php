<?php

include "test_inc.php";

class IssuesTest extends ScssUnitTest {

    public function test_extend_issue80(){
        $source = <<<END
.test {
	color: green;
	&:hover { color: red; }
}
.test2 {
    @extend .test;
}
END;
        $expected = <<<END
.test, .test2 {
  color: green;
}

.test:hover, .test2:hover {
  color: red;
}
END;
        $this->assertScss($source, $expected);
    }

    public function test_extend3(){
        $source = <<<END
.test {
	color: green;
	a { color: red; }
}
.test2 {
    @extend .test;
}
END;
        $expected = <<<END
.test, .test2 {
  color: green;
}

.test a, .test2 a {
  color: red;
}
END;
        $this->assertScss($source, $expected);
    }
/*
    public function test_extend2(){
        $source = <<<END
.error {
  border: 1px #f00;
  background-color: #fdd;
}
.seriousError {
  @extend .error;
  border-width: 3px;
}
END;
        $expected = <<<END
.test, .test2 {
  color: green;
}

.test:hover, .test2:hover {
  color: red;
}
END;
        $this->assertScss($source, $expected);
    }

    public function test_multiple_extends(){
        $source = <<<END
.error {
  border: 1px #f00;
  background-color: #fdd;
}
.attention {
  font-size: 3em;
  background-color: #ff0;
}
.seriousError {
  @extend .error;
  @extend .attention;
  border-width: 3px;
}
END;
        $expected = <<<END
.error, .seriousError {
  border: 1px #f00;
  background-color: #fdd; }

.attention, .seriousError {
  font-size: 3em;
  background-color: #ff0; }

.seriousError {
  border-width: 3px; }
END;
        $this->assertScss($source, $expected);
    }
*/
}


$unit = new IssuesTest();
$unit->run();
