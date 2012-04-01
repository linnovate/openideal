<style type="text/css">
<!--
body {
	font: 100%/1.4 Verdana, Arial, Helvetica, sans-serif;
	margin: 0;
	padding: 0;
}

table.testunit {
	margin: 0 0 10px 0;
	border: 1px solid #CCC;
}
table.testunit th {
	border: 1px solid #CCC;
	padding: 4px;
	background-color:#FFC;
}
table.testunit td {
	border: 1px solid #CCC;
	padding: 4px;
	vertical-align:top;
}
-->
</style>

<?php

function p(){
    $args = func_get_args();
    $n = sizeof($args);
    for($i = 0; $i < $n; $i++){
        $out = print_r($args[$i], true);
        $s1 = array(" ", "\n");
        $s2 = array("&nbsp;", "<br>");
        echo str_replace($s1,$s2,$out);
        echo "<br />\n";
    }
}

class ScssUnitTest {
    protected $sass;
    public function __construct(){
        require_once(dirname(__FILE__).'/../sass/SassParser.php');
        $this->sass = new SassParser(array(
            'extensions' => array('compass'=>array()),
            'style' => SassRenderer::STYLE_EXPANDED,
            'syntax' => SassFile::SCSS,
        ));
    }

    public function assertScss($scss, $css){
        $out = '<table class="testunit">';
        $out .= '<tr>';
        $out .= '<th>source Scss</th>';
        $out .= '<th>Result</th>';
        $out .= '<th>Output css</th>';
        $out .= '<th>expected css</th>';
        $out .= '</tr>';

        if(!is_array($scss)){
            $scss = array($scss);
            $css = array($css);
        }
        for($i=0, $k=count($scss); $i<$k; $i++){
            $css[$i] = trim($css[$i]);
            try {
                $result = $this->sass->toCss($scss[$i], false);
                $result = trim($result);
            }
            catch(Exception $e){
                $result = $e->getMessage();
            }
            $css[$i] = str_replace(array("\r\n", "\n\r", "\r"), "\n", $css[$i]);
            $pass = ($css[$i] == $result);
            $out .= '<tr>';
            $out .= '<td><pre>'.htmlspecialchars($scss[$i]).'</pre></td>';
            $out .= '<td style="background-color:'.($pass?'#CFC':'#FCC').';">'.($pass?'<h5>Pass!</h5>':'<h5>Failed!</h5>').'</td>';
            $out .= '<td style="background-color:'.($pass?'#CFC':'#FCC').';">'.'<pre>'.htmlspecialchars($result).'</pre></td>';
            $out .= '<td><pre>'.htmlspecialchars($css[$i]).'</pre></td>';
            $out .= '</tr>';
        }
        $out .= '</table>';
        echo $out;
    }

    public function run(){
        $methods = get_class_methods($this);
        foreach($methods as $method){
            if(substr($method, 0, 4) == "test"){
                echo '<h3>'.$method.'</h3>';
                $this->$method();
            }
        }
    }
}
