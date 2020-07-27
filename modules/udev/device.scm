(define-module (udev device)
  #:export (udev-device
	    udev-device?
	    udev-device-get-action
	    udev-device-get-sysname
	    udev-device-get-devpath
	    udev-device-get-devnode
	    udev-device-get-devtype
            udev-device-get-driver
            udev-device-get-udev
            udev-device-get-property-value
            udev-device-get-links
            udev-device-get-properties))

(load-extension "libguile-udev" "init_udev_device")
