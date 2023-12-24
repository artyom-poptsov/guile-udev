#!/usr/bin/env -S guile -L modules -e main -s
!#

(use-modules (udev udev)
             (udev monitor)
             (udev device))

(define (callback device)
  (display "device: ")
  (display (udev-device-get-devpath device))
  (newline))

(define (main args)
  (let* ((udev         (make-udev))
         (udev-monitor (make-udev-monitor udev
                                          #:callback callback
                                          #:filter (list "usb" "usb_device"))))
    (udev-monitor-start-scanning! udev-monitor)
    (while #t
      (sleep 1))))
