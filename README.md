Projekt 1.

Temat: Prosta powłoka tekstowa (shell)

**************************************************************************************************************************************

Autor: Paweł Szapiel 

Przedmiot: Systemy Operacyjne 
_______________________________________________________________________________________________________________________________________



Opis:   Program wczytuje komendy ze standardowego wejścia lub z pliku, w którym w pierwszej linii widnieje "#![PATH_TO_SHELL]".
        Skrypty z poza programu nie są obsługiwane. Powłoka obsługuje wszystkie programy z katalogu /bin oraz dowolną liczbę argumentów 
        pisanych łącznie lub rozdzielnie. Każdą komendę należy zatwierdzić znakiem końca linii. 
        
        Powłoka działa według schematu: 
        while True {
            czytaj wejście;
            rozdziel ciąg znaków na tablice stringów 
                    tutaj obsługa znaków { '|', '>>', '&'} i odpowiadającym im funkcjonalności
            wykonaj programy basha (używana komenda z rodziny exec)
        }  

Zaimplementowane funkcjonalności:

    * możliwość przekierowania standardowego wyjścia polecenia przy pomocy »
    * możliwość tworzenia potoków o dowolnej długości przy pomocy znaku |
    * przechowuje w zewnętrznym pliku w katalogu /src dokładną treść 20 poleceń od ostatniego na samej górze
      do najwcześniejszego na samym dole - historia nie jest czyszczona po zamknięciu programu,
      a wysłanie sygnału SIGQUIT powoduje wyświetlenie historii na standardowym wyjściu.

Struktura projektu:

newshell__
          |_include_
          |         |_shell.h
          |_src_____
                    |_bin____
                    |        |_*.o
                    |
                    |_src____
                             |_*.c
                             |_shell

                        
                



