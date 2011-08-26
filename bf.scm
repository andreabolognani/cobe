; bf.scm -- Simple Brainfuck interpreter
; Copyright (C) 2010-2011  Andrea Bolognani <andrea.bolognani@roundhousecode.com>
;
; This program is free software; you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation; either version 2 of the License, or
; (at your option) any later version.
;
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public License along
; with this program; if not, write to the Free Software Foundation, Inc.,
; 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

(define bf-load #f)
(define bf-run  #f)

(let ()

  (define ops '(#\+ #\- #\> #\< #\. #\,))

  (define (make-tape p c n)
    (cons p (cons c n)))

  (define (tape-plus tape)
    (let ((c (cadr tape)))
      (make-tape (car tape)
                 (if (= c 127) 0 (+ c 1))
                 (cddr tape))))

  (define (tape-minus tape)
    (let ((c (cadr tape)))
      (make-tape (car tape)
                 (if (= c 0) 127 (- c 1))
                 (cddr tape))))

  (define (tape-right tape)
    (let ((c (cadr tape))
          (p (car tape))
          (n (cddr tape)))
      (make-tape (cons c p)
                 (if (null? n) 0 (car n))
                 (if (null? n) '() (cdr n)))))

  (define (tape-left tape)
    (let ((c (cadr tape))
          (p (car tape))
          (n (cddr tape)))
      (make-tape (if (null? p) '() (cdr p))
                 (if (null? p) 0 (car p))
                 (cons c n))))

  (define (tape-write tape)
    (display (integer->char (cadr tape)))
    tape)

  (define (tape-read tape)
    (let* ((c (read-char (current-input-port)))
           (c (if (eof-object? c) 0 (char->integer c)))
           (p (car tape))
           (n (cddr tape)))
      (make-tape p c n)))

  (define (load/real port)
    (load/code port '()))

  (define (load/code port code)
    (let ((c (read-char port)))
      (cond
        ((eof-object? c) (reverse code))
        ((equal? c #\])  (reverse code))
        ((equal? c #\[)  (load/code port (cons (load/code port '()) code)))
        ((member c ops)  (load/code port (cons c code)))
        (else            (load/code port code)))))

  (define (run/real code)
    (run/tape code (make-tape '() 0 '())))

  (define (run/tape code tape)
    (if (null? code)
        tape
        (let ((i (car code)))
          (cond
            ((equal? i #\.) (run/tape (cdr code) (tape-write tape)))
            ((equal? i #\,) (run/tape (cdr code) (tape-read tape)))
            ((equal? i #\+) (run/tape (cdr code) (tape-plus tape)))
            ((equal? i #\-) (run/tape (cdr code) (tape-minus tape)))
            ((equal? i #\>) (run/tape (cdr code) (tape-right tape)))
            ((equal? i #\<) (run/tape (cdr code) (tape-left tape)))
            ((list? i)      (if (> (cadr tape) 0)
                                (run/tape code (run/tape i tape))
                                (run/tape (cdr code) tape)))))))

  (set! bf-load load/real)
  (set! bf-run  run/real))

(define (bf filename)
  (with-input-from-file filename
    (lambda ()
      (let ((program (bf-load (current-input-port))))
        (bf-run program)))))

(define (main args)
  (if (< (length args) 2)
      (begin
        (display "Usage: ")
        (display (car args))
        (display " PROGRAM")
        (newline)
        1)
      (begin
        (bf (cadr args))
        0)))

(exit (main (program-arguments)))

; vim: set filetype=scheme :
