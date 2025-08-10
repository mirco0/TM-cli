% Programma per determinare lunghezza pari di una stringa
<q0,x,x,qPari,Stop> \forall x \in \Sigma
<qPari,x,x,qDispari,Right> \forall x \in \Sigma
<qDispari,x,x,qPari,Right> \forall x \in \Sigma
<q,\square,q,qF,Stop> \forall q \in \{qPari,qDispari\}