<q0,\square,0,q1,Right>
<q1,\square,1,q2,Right>
<q2,\square,0,q3,Right>
<q3,\square,0,q4,Right>
<q4,\square,1,q5,Right>
<q5,\square,0,q6,Right>
<q6,\square,1,q7,Right>
<q7,\square,0,qR,Left>

<qR,x,x,qR,Left> \forall x \in \Sigma
<qR,\square,\square,qPari,Right>

<qPari,x,x,qDispari,Right> \forall x \in \Sigma
<qDispari,x,x,qPari,Right> \forall x \in \Sigma
<q,\square,q,qF,Stop> \forall q \in \{qPari,qDispari\}