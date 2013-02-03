set logscale x
set xrange [0.1:500000]
set yrange [-40:10]
H(w)=1/(({0,1}*w/5000.0)**2+0.6*{0,1}*w/5000.0+1)
A(w)=abs(H(w))
db(w)=20*log10(A(w))
plot db(x)
