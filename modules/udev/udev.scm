(define-module (udev udev)
  #:export (udev
	    udev?
	    make-udev))

(load-extension "libguile-udev" "init_udev")
