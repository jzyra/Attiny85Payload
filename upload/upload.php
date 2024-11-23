<?php
define("PATH_UPLOAD", getcwd() . "/exfiltrate");

function get_filename() {
  return PATH_UPLOAD . "/" . date('Y-m-d_H-i-s') . "_" . rand(0, 10000);
}

function main() {
  if (is_writable(PATH_UPLOAD)) {
    file_put_contents(get_filename(), file_get_contents("php://input"));
  } else {
    echo PATH_UPLOAD . " is not writable.";
    exit(1);
  }
}

main();
?>