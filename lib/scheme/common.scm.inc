; common.scm.inc - Useful Scheme routines
; Copyright (C) 2011  Andrea Bolognani <andrea.bolognani@roundhousecode.com>
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

(define (foldl pred initial-value lst)
  (define (doit v l)
            (if (null? l)
                v
                (doit (pred (car l) v) (cdr l))))
  (doit initial-value lst))

(define (range from to)
  (define (doit i acc)
    (if (< i from)
        acc
        (doit (- i 1) (cons i acc))))
  (doit to (list)))

(define (list-merge l1 l2)
  (define (doit a b)
    (if (null? a)
        b
        (doit (cdr a)
              (cons (car a) b))))
  (doit (reverse l1) l2))

(define (list-flatten lst)
  (foldl (lambda (e a)
           (list-merge e a))
         (list)
         lst))

(define (list-remove lst i)
  (define (doit l c acc)
    (if (zero? c)
        (append (reverse acc)
                (cdr l))
        (doit (cdr l)
              (- c 1)
              (cons (car l) acc))))
  (doit lst i (list)))

(define (list-remove-duplicates lst)
  (define (doit l a)
    (if (null? l)
        (reverse a)
        (if (not (member (car l) (cdr l)))
            (doit (cdr l) (cons (car l) a))
            (doit (cdr l) a))))
  (doit lst (list)))

; Read a whole line (trailing newline is stripped) from a port,
; or from (current-input-port) if no port is specified
(define (read-line . args)

  (define (doit port acc)
    (let ((c (read-char port)))
      (cond
        ((and (eof-object? c) (null? acc))         c)
        ((or (eof-object? c) (equal? #\newline c)) (list->string (reverse acc)))
        (else                                      (doit port (cons c acc))))))

  (if (null? args)
      (doit (current-input-port) (list))
      (doit (car args) (list))))

; vim: set filetype=scheme :
