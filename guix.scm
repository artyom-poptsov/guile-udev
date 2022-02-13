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
    (native-inputs
     (list autoconf
           automake
           gettext-minimal
           libtool
           texinfo
           pkg-config
           which))
    (inputs
     (list guile-3.0 eudev))
    (home-page "https://github.com/artyom-poptsov/guile-udev")
    (synopsis "Guile bindings to libudev")
    (description
     "Guile-Udev provides GNU Guile bindings to libudev.")
    (license license:gpl3+)))

guile-udev

;;; guix.scm ends here.

