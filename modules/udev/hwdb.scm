(define-module (udev hwdb)
  #:export (udev-hwdb
	    udev-hwdb?
            make-udev-hwdb
            udev-hwdb-lookup))

(load-extension "libguile-udev" "init_udev_hwdb")
