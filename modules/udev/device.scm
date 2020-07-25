(define-module (udev device)
  #:export (udev-device
	    udev-device?
	    udev-device-get-action
	    udev-device-get-sysname
	    udev-device-get-devpath
	    udev-device-get-devnode
	    udev-device-get-devtype
            udev-device-get-driver))

(load-extension "libguile-udev" "init_udev_device")
