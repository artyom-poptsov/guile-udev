;;; monitor.scm -- Testing of Guile-Udev monitor.

;; Copyright (C) 2022 Artyom V. Poptsov <poptsov.artyom@gmail.com>
;;
;; This file is a part of Guile-Udev.
;;
;; Guile-Udev is free software: you can redistribute it and/or
;; modify it under the terms of the GNU General Public License as
;; published by the Free Software Foundation, either version 3 of the
;; License, or (at your option) any later version.
;;
;; Guile-Udev is distributed in the hope that it will be useful, but
;; WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
;; General Public License for more details.
;;
;; You should have received a copy of the GNU General Public License
;; along with Guile-Udev.  If not, see <http://www.gnu.org/licenses/>.

(add-to-load-path (getenv "abs_top_srcdir"))

(use-modules (srfi srfi-64)
             (udev udev)
             (udev monitor))


(define %test-suite-name "monitor")

(test-begin %test-suite-name)



(test-assert "make-udev-monitor"
  (let* ((udev (make-udev))
         (monitor (make-udev-monitor udev)))
    monitor))


(define exit-status (test-runner-fail-count (test-runner-current)))

(test-end %test-suite-name)

(exit exit-status)

;;; monitor.scm ends here.
