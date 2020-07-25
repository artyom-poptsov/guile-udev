(define-module (udev monitor)
  #:export (udev-monitor
	    udev-monitor?
	    make-udev-monitor
	    udev-monitor-add-filter!
	    udev-monitor-set-callback!
	    udev-monitor-start-scanning!
	    udev-monitor-stop-scanning!
            udev-monitor-get-udev))

(load-extension "libguile-udev" "init_udev_monitor")
