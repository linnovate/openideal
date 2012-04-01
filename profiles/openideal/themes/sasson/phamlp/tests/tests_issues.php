<?php

include "test_inc.php";

class IssuesTest extends ScssUnitTest {

    public function test_lighten_darken(){
        $tests = array(
            '{ color: lighten(#B8860B, 3%); }' => 'color: #c6910c;',
            '{ color: darken(#b8860b, 3%); }' => 'color: #aa7b0a;',
            '{ color: lighten(#99FF99, 8%); }' => 'color: #c2ffc2;',
            '{ color: darken(#99FF99, 8%); }' => 'color: #70ff70;',
            '{ color: lighten(#A4D9E4, 6%); }' => 'color: #bce3eb;',
            '{ color: darken(#A4D9E4, 6%); }' => 'color: #8ccfdd;',
            '{ color: lighten(#806918, 26%); }' => 'color: #dbb941;',
            '{ color: darken(#806918, 8%); }' => 'color: #5e4d12;',
        );
        $this->assertScss(array_keys($tests), array_values($tests));
    }

    public function test_extend_issue54(){
        $source = <<<END
\$test-var: 1;

@mixin catch-my-error(\$align: right) {
	\$align: unquote(\$align);
	@if \$align == right {
		.test-right-cls {
			text-align: right;
        }
    }
	@else if \$align == left {
		.test-left-cls {
			text-align: left;
        }
    }
    @else {
		.test-else-cls {
			text-align: none;
        }
    }
}

.cls-1 {
	@include catch-my-error(right);
}
.cls-2 {
	@include catch-my-error(left);
}
.cls-3 {
	@if \$test-var == 2 {
		padding: 10px;
    }
	@else if \$test-var == 1 {
		.test-1-cls {
			padding: 0;
        }
    }
}
END;
        $expected = <<<END
.cls-1 .test-right-cls {
  text-align: right;
}


.cls-2 .test-left-cls {
  text-align: left;
}


.cls-3 .test-1-cls {
  padding: 0;
}
END;
        $this->assertScss($source, $expected);
    }

}


$unit = new IssuesTest();
$unit->run();
