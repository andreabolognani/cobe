; anagrams.scm - Find and print all anagrams of a word
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

; Include generic routines
(load "../lib/scheme/common.scm.inc")

; Dictionary file to use
(define dict-file "/usr/share/dict/words")

; Size limits
(define character-set-length 127)
(define printable-offset     32)
(define fingerprint-range    (- character-set-length printable-offset))

; Create a fingerprint for a string.
;
; The fingerprint records how many time each character belonging to
; the ASCII character set appears in a string, ignoring the ordering.
;
; All characters not belonging to ASCII are collapsed into a single
; counter, instead of being ignored, so that n\'ee does not have the
; same fingerprint as ne.
(define (string->fingerprint str)

  (define (doit lst vec)
    (if (null? lst)
        vec
        (let* ((c (car lst))
               (i (- (char->integer c) printable-offset))
               (i (if (and (>= i 0) (< i fingerprint-range)) i fingerprint-range))
               (v (vector-ref vec i)))
          (vector-set! vec i (+ v 1))
          (doit (cdr lst) vec))))

  (doit (string->list str)
        (make-vector (+ fingerprint-range 1) 0)))

; Calculate length of a string matching the fingerprint
(define (fingerprint-length fingerprint)
  (apply + (vector->list fingerprint)))

; Find dictionary words matching a fingerprint.
;
; Since fingerprint construction is a bit on the expensive
; side, it is skipped if the lenght of the dictionary word
; doesn't match the length of the fingerprint
(define (fingerprint-match fingerprint)

  ; Cache fingerprint length, no need to compute
  ; that for every iteration
  (define fingerlen (fingerprint-length fingerprint))

  (define (doit anagrams)
    (let ((line (read-line)))
      (if (eof-object? line)
          anagrams
          (if (not (= (string-length line) fingerlen))
              (doit anagrams)
              (let ((f (string->fingerprint (string-downcase line))))
                (if (equal? f fingerprint)
                    (doit (cons line anagrams))
                    (doit anagrams)))))))

  (with-input-from-file dict-file
                        (lambda ()
                          (doit (list)))))

; List all anagrams of a word
(define (anagrams word)
  (let* ((word        (string-downcase word))
         (fingerprint (string->fingerprint word)))
    (fingerprint-match fingerprint)))

; Program entry point
(define (main args)
  (if (< (length args) 2)
      (begin
        (display "Usage: ")
        (display (car args))
        (display " WORD")
        (newline)
        1)
      (let ((words (anagrams (cadr args))))
        (foldl (lambda (word retval)
                 (display word)
                 (newline)
                 0)
               1
               words))))

(exit (main (program-arguments)))

; vim: set filetype=scheme :
