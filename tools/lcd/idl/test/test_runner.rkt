#lang racket


;; just check if extention is .idl
;; all paths relative to current directory
(define (is-test-case? path)
  (let* ([split-path (string-split (path->string path) "/")]
        [reversed (reverse split-path)])
    (if (empty? (cdr reversed))
        #f
        (and (equal? (cadr reversed) "test_cases")
             (equal? (car (reverse (string-split (car reversed) ".")))
                     "idl")))))

(define exec (string->path "../compiler"))

(define test-cases
 ;; (with-handlers ([exn:fail? (lambda (exn) '())])
  (map (lambda (path)
         (build-path (current-directory) path))
       (find-files is-test-case? #f)))

(define (run-server-header-test tests)
  (if (empty? tests)
      (displayln "no tests")
      (let* ([test (path->string (car tests))]
             [out-file (build-path (current-directory) "test_cases" (string-append (car (reverse (string-split test "/")))
                                                          ".server.h"))])
        (displayln `(server header test: ,test) (current-error-port))
        (system* exec "-serverheader" test out-file))))
      

(define (run-server-source-test tests)
    (if (empty? tests)
      (displayln "no tests")
      (let* ([test (path->string (car tests))]
             [out-file (build-path (current-directory) "test_cases" (string-append (car (reverse (string-split test "/")))
                                                          ".server.c"))])
        (displayln `(server source test: ,test) (current-error-port))
        (system* exec "-serversource" test out-file))))

(define (run-client-header-test tests)
    (if (empty? tests)
      (void)
      (let* ([test (path->string (car tests))]
             [out-file (build-path (current-directory) "test_cases" (string-append (car (reverse (string-split test "/")))
                                                          ".client.h"))])
        (displayln `(client header test: ,test) (current-error-port))
        (system* exec "-clientheader" test out-file))))

(define (run-client-source-test tests)
    (if (empty? tests)
      (void)
      (let* ([test (path->string (car tests))]
             [out-file (build-path (current-directory "test_cases" (string-append (car (reverse (string-split test "/")))
                                                          ".client.c")))])
        (displayln `(client source test: ,test) (current-error-port))
        (system* exec "-clientsource" test out-file))))


(define (run-tests tests)
  #|
  (displayln "Running server header test")
  (run-server-header-test tests))
|#
  (displayln "Running server source test")
  (run-server-source-test tests))
  #|
  (displayln "Running client header test")
  (run-client-header-test tests)
  (displayln "Running client source test")
  (run-client-source-test tests)) |#

(run-tests test-cases)