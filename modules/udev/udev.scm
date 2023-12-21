;;; udev.scm -- Udev procedures.
;;
;; Copyright (C) 2020-2023 Artyom V. Poptsov <poptsov.artyom@gmail.com>
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

;; This module contains procedures for working with Udev instances.


;;; Code:

(define-module (udev udev)
  #:export (udev
	    udev?
	    make-udev))


(load-extension "libguile-udev" "init_udev")

;;; udev.scm ends here.
