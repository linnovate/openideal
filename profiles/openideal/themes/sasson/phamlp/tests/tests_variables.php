<?php

include "test_inc.php";

class IssuesTest extends ScssUnitTest {

    public function test_variables(){
        $source = <<<END
\$var1: #333333;
\$var2: \$var1 + #123123;
a {
  color: \$var1;
  background: \$var2;
}
END;
        $expected = <<<END
a {
  color: #333333;
  background: #456456;
}
END;
        $this->assertScss($source, $expected);
    }

}


$unit = new IssuesTest();
$unit->run();
