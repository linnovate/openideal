#!/usr/bin/php -q
<?php

error_reporting(E_ALL);
$path  = realpath(dirname(__FILE__)).'/';

function error($msg){
    fwrite(STDERR, $msg."\n");
}



if(php_sapi_name() != "cli"){
	error($argv[0]." must be run in the command line.");
	exit(1);
}

$command = array_shift($argv);

if(isset($argv[0])){
    if($argv[0] == 'watch'){
        if(isset($argv[1])){
            if(is_dir($argv[1]) || is_file($argv[1])){
                echo "Phamlp sass is polling for changes. Press Ctrl-C to Stop.\n";
                $sc = new SassCompiler;
				$sc->setPath($argv[1]);
                $sc->loop();
			}
            else {
                error($argv[1]." is not a directory or sass file.\n");
			}
        }
        else
            error("Falta o caminho: sassc watch [path]");

    }
    else
        error("Falta comando: sassc commando");
}
else{
    error("Falta o caminho: sassc watch [path]");
}


class SassCompiler {
    private $_sass;
	private $_path;
    private $_updates;

    public function __construct(){
        require_once(dirname(__FILE__).'/sass/SassParser.php');
        $this->_sass = new SassParser(array(
            'extensions' => array('compass'=>array()),
            'style' => SassRenderer::STYLE_EXPANDED,
            'syntax' => SassFile::SCSS,
        ));
    }

    public function loop(){
    	while(1){
			$this->compile();
			sleep(1);
		}
	}

    public function compile(){
        try {
        	clearstatcache();
        	foreach($this->getFiles() as $i => $file){
        		$fname = $file['filename'];
        		if(!isset($this->_updates[$fname]) || filemtime($file['filename']) > $this->_updates[$fname]){
	        		$c = file_get_contents($file['filename']);
					if(empty($c)) continue;
	            	$result = $this->_sass->toCss($c, false);
					$newfile = substr($fname, 0, -5).'.css';
					file_put_contents($newfile, $result);
					$this->_updates[$fname] = filemtime($file['filename']);
					echo "save file $newfile\n";
				}
            }
        }
        catch(Exception $e){
            $result = $e->getMessage();
			echo "Error: ".$result;
        }
    }


    public function setPath($path){
		$this->_path = $path;
	}

	public function getFiles(){
		$files = array();
        if(is_dir($this->_path)){
            if(($handle = opendir($this->_path))){
                while(($file = readdir($handle)) !== false){
                    if(substr($file, -5) === '.scss' && is_file($this->_path.DIRECTORY_SEPARATOR.$file)){
                        $files[] = array('filename'=>$this->_path.DIRECTORY_SEPARATOR.$file);
                    }
                }
			}
        }
		else if(is_file($this->_path)){
        	$files[] = array('filename'=>$file);
		}
		return $files;
	}

    public function addDir($dir){
        if(is_dir($dir)){
            if(($handle = opendir($dir))){
                while(($file = readdir($handle)) !== false){
                    if(substr($file, -5) === '.scss' && is_file($dir.DIRECTORY_SEPARATOR.$file)){
                        $this->_files[] = array('filename'=>$dir.DIRECTORY_SEPARATOR.$file);
                    }
                }
			}
        }
    }

}
