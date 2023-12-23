;;; monitor.scm -- Udev monitor procedures.
;;
;; Copyright (C) 2020-2023 Artyom V. Poptsov <poptsov.artyom@gmail.com>
;; Copyright (C) 2023 Maxim Cournoyer <maxim.cournoyer@gmail.com>
;;
;; This file is part of Guile-Udev.
;;
;; Guile-Udev is free software: you can redistribute it and/or modify it under
;; the terms of the GNU General Public License as published by the Free
;; Software Foundation, either version 3 of the License, or (at your option)
;; any later version.
;;
;; Guile-Udev is distributed in the hope that it will be useful, but WITHOUT
;; ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
;; FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
;; more details.
;;
;; You should have received a copy of the GNU General Public License along
;; with Guile-Udev. If not, see <http://www.gnu.org/licenses/>.


;;; Commentary:

;; This module contains procedures for working with Udev monitors.


;;; Code:

(define-module (udev monitor)
  #:export (udev-monitor
	    udev-monitor?
	    %make-udev-monitor
            make-udev-monitor
	    udev-monitor-add-filter!
            udev-monitor-remove-filters!
            udev-monitor-set-timeout!
	    udev-monitor-set-callback!
            udev-monitor-set-error-callback!
	    udev-monitor-start-scanning!
	    udev-monitor-stop-scanning!
            udev-monitor-get-udev))


;; A high-level procedure that creates a new udev monitor instance with the
;; specified parameters.
(define* (make-udev-monitor udev
                            #:key
                            (callback     (const #t))
                            (error-callback (lambda (monitor error-message)
                                              (format (current-error-port)
                                                      "ERROR: in ~a: ~a~%"
                                                      monitor error-message)))
                            (filter       #f)
                            (timeout-usec 0)
                            (timeout-sec  0))
  "Create a new 'udev-monitor' object configured with the specified parameters.
CALLBACK is a one argument procedure (receiving a 'udev-device' object) called
when an event matching the specified FILTER.  FILTER is a list whose first
element is the device subsystem, and whose second argument is the device type,
or #f to match any type."
  (let ((monitor (%make-udev-monitor udev)))
    (udev-monitor-set-timeout!  monitor timeout-sec timeout-usec)
    (udev-monitor-set-callback! monitor callback)
    (udev-monitor-set-error-callback! monitor error-callback)
    (when filter
      (let ((subsystem (car filter))
            (devtype (cadr filter)))
        (if devtype
            (udev-monitor-add-filter! monitor subsystem devtype)
            (udev-monitor-add-filter! monitor subsystem))))
    monitor))

(load-extension "libguile-udev" "init_udev_monitor")

;;; monitor.scm ends here.
