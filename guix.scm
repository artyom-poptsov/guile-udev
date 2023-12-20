;; guix.scm --- GNU Guix package recipe    -*- coding: utf-8 -*-
;;
;; Copyright (C) 2022-2023 Artyom V. Poptsov <poptsov.artyom@gmail.com>
;;
;; This file is part of Guile-Udev.
;;
;; This program is free software: you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation, either version 3 of the License, or
;; (at your option) any later version.
;;
;; The program is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.
;;
;; You should have received a copy of the GNU General Public License
;; along with the program.  If not, see <http://www.gnu.org/licenses/>.


;;; Commentary:
;;
;; GNU Guix development package. To use as the basis for a development
;; environment, run:
;;
;;  guix environment --pure --container -l guix.scm
;;
;; In the new shell, run:
;;
;;  autoreconf -vif && ./configure && make check
;;
;;; Code:

(use-modules (guix gexp)
             ((guix licenses) #:prefix license:)
             (guix build-system gnu)
             (guix git-download)
             (guix packages)
             (gnu packages autotools)
             (gnu packages base)
             (gnu packages gettext)
             (gnu packages guile)
             (gnu packages linux)
             (gnu packages pkg-config)
             (gnu packages texinfo))


(define %source-dir (dirname (current-filename)))


(define-public guile-udev
  (package
    (name "guile-udev")
    (version "git")
    (source (local-file %source-dir
                        #:recursive? #t
                        #:select? (git-predicate %source-dir)))
    (build-system gnu-build-system)
    (arguments
     (list
      #:phases
      #~(modify-phases %standard-phases
          (add-after 'unpack 'patch-extension-path
            (lambda* (#:key outputs #:allow-other-keys)
              (substitute* (find-files "." "\\.scm")
                (("\\(load-extension \"libguile-udev\" *\"(.*)\"\\)" _ o)
                 (string-append
                  (object->string
                   `(or (false-if-exception
                         (load-extension "libguile-udev" ,o))
                        (load-extension
                         ,(string-append
                           #$output
                           "/lib/libguile-udev.so")
                         ,o)))))))))))
    (native-inputs
     (list autoconf
           automake
           gettext-minimal
           libtool
           texinfo
           pkg-config
           which
           guile-3.0))                  ; Needed for cross-compiling
    (inputs
     (list guile-3.0 eudev))
    (home-page "https://github.com/artyom-poptsov/guile-udev")
    (synopsis "Guile bindings to libudev")
    (description
     "Guile-Udev provides GNU Guile bindings to libudev.")
    (license license:gpl3+)))

guile-udev

;;; guix.scm ends here.
