(define-module (udev monitor)
  #:export (udev-monitor
	    udev-monitor?
	    make-udev-monitor
	    udev-monitor-add-filter!
	    udev-monitor-set-callback!
	    udev-monitor-start-scanning!
	    udev-monitor-stop-scanning!))

(load-extension "libguile-udev" "init_udev_monitor")
