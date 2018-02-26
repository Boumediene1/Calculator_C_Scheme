;;;IFT 2035  TP2 - Programmation fonctionnelle.

;;;Boumediene Boukharouba
;;;20032279

;;;Alexandre Deneault
;;;20044305


;;; Fichier : tp2.scm

;;; Ce programme est une version incomplete du TP2.  Vous devez uniquement
;;; changer et ajouter du code dans la première section.

;;;----------------------------------------------------------------------------

;;; Vous devez modifier cette section.  La fonction "traiter" doit
;;; être définie, et vous pouvez ajouter des définitions de fonction
;;; afin de bien décomposer le traitement à faire en petites
;;; fonctions.  Il faut vous limiter au sous-ensemble *fonctionnel* de
;;; Scheme dans votre codage (donc n'utilisez pas set!, set-car!,
;;; begin, etc).

;;; La fonction traiter reçoit en paramètre une liste de caractères
;;; contenant la requête lue et le dictionnaire des variables sous
;;; forme d'une liste d'association.  La fonction retourne
;;; une paire contenant la liste de caractères qui sera imprimée comme
;;; résultat de l'expression entrée et le nouveau dictionnaire.  Vos
;;; fonctions ne doivent pas faire d'affichage car c'est la fonction
;;; "repl" qui se charge de cela.

(define traiter
  (lambda (expr dict)  					;recoit une list de caracteres et un dictionnaire avec les variable dedans
    (let ((resultat (lire expr '() dict)))
      (if (car resultat)
        resultat
        (cons (cdr resultat) dict)))))



;; Retourne une paire contenant la liste de caractères à afficher et le nouveau dictionnaire.
;; Retourne une paire contenant #f et le message d'erreur à afficher s'il y a une erreur.
(define lire
  (lambda (expr pile dict)
    (cond 	((null? expr)                                                                ;Lorsque la lecture de l'expression est terminé
		  (cond		((null? pile)                                                      ;Si il n'y a rien sur la pile
				  (cons #f (string->list "L'entree est vide.\n")))
				((not (null? (cdr pile)))                                          ;Si il y a plus d'un nombre sur la pile
				  (cons #f (string->list "Il n'y a pas assez d'operateur.\n")))
				(else
				  (cons (afficher (car pile)) dict))))
		((eof-object? expr)
		  (exit))
		((equal? (car expr) #\space)
		  (lire (cdr expr) pile dict))
		((char-numeric? (car expr))
		  (lire-chiffre expr 0  (lambda (succes expr n)
					  (if (car succes)
					    (lire expr (cons n pile) dict)
					    succes))))
		((equal? (car expr) #\+)
		  (operation + pile (lambda (succes pile)
				      (if (car succes)
					(lire (cdr expr) pile dict)
					succes))))
		((equal? (car expr) #\-)
		  (operation - pile (lambda (succes pile)
				      (if (car succes)
					(lire (cdr expr) pile dict)
					succes))))
		((equal? (car expr) #\*)
		  (operation * pile (lambda (succes pile)
				      (if (car succes)
					(lire (cdr expr) pile dict)
					succes))))
		((and (char-lower-case? (car expr)) (or (null? (cdr expr)) (equal? (cadr expr) #\space)))
		  (let ((paire (assoc (car expr) dict)))
		    (if paire
		      (lire (cdr expr) (cons (cdr paire) pile) dict)
		      (cons #f (string->list "Une variable n'a pas ete initialise.\n")))))
		((equal? (car expr) #\=)
		  (if (and (not (null? (cdr expr))) (char-lower-case? (cadr expr)) (or (null? (cddr expr)) (equal? (caddr expr) #\space)) (not (null? pile)))
		    (if (assoc (cadr expr) dict)
		      (lire (cddr expr) pile (cons (cons (cadr expr) (car pile)) (foldl (lambda (x y) (if (not (equal? (cadr expr) (car y))) (cons y x) x)) '() dict)))
		      (lire (cddr expr) pile (cons (cons (cadr expr) (car pile)) dict)))
		    (cons #f (string->list "Il y a une mauvaise affectation de variable.\n"))))
		(else
		  (cons #f (string->list "Un caractere non reconnu a ete lu.\n"))))))



(define afficher
  (lambda (n)
    (append (string->list (number->string n)) '(#\newline))))


(define lire-chiffre
  (lambda (expr chiffre cont)
    (cond	((or (null? expr) (equal? (car expr) #\space))
		  (cont (cons #t "") expr chiffre))
		((char-numeric? (car expr))
		  (lire-chiffre (cdr expr)  (+ (* 10 chiffre) (- (char->integer (car expr)) (char->integer #\0))) cont))
		(else
		  (cont (cons #f (string->list "Un caractere invalide a ete lu.\n")) expr chiffre)))))


(define operation
  (lambda (op pile cont)
    (if (not (or (null? pile) (null? (cdr pile))))
      (cont (cons #t "")  (cons   (op (cadr pile) (car pile))
                                  (cddr pile)))
      (cont (cons #f (string->list "Il n'y a pas assez d'operande.\n")) pile))))



;;; Vu en classe.
(define foldl
  (lambda (f base lst)
    (if (null? lst)
      base
      (foldl f (f base (car lst)) (cdr lst)))))

;;;----------------------------------------------------------------------------

;;; Ne pas modifier cette section.

(define repl
  (lambda (dict)
    (print "# ")
    (let ((ligne (read-line)))
      (if (string? ligne)
          (let ((r (traiter-ligne ligne dict)))
            (for-each write-char (car r))
            (repl (cdr r)))))))

(define traiter-ligne
  (lambda (ligne dict)
    (traiter (string->list ligne) dict)))

(define main
  (lambda ()
    (repl '()))) ;; dictionnaire initial est vide

;;;----------------------------------------------------------------------------
(main)