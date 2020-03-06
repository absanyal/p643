set terminal pdfcairo

set output "natural.pdf"
set title "Natural frequency"
set xlabel "t"
set ylabel "y"
set size square
plot "natural.dat" u 1:2 w l
unset output

set output "natural_phase.pdf"
set title "Natural frequency, phase space"
set xlabel "y"
set ylabel "y'(t)"
set size square
plot "natural.dat" u 2:3 w l
unset output

set output "slightly_damped.pdf"
set title "Slightly damped"
set xlabel "t"
set ylabel "y"
set size square
plot "slightly_damped.dat" u 1:2 w l
unset output

set output "slightly_damped_phase.pdf"
set title "Slightly damped, phase space"
set xlabel "y"
set ylabel "y'(t)"
set size square
plot "slightly_damped.dat" u 2:3 w l
unset output

set output "heavily_damped.pdf"
set title "Heavily damped"
set xlabel "t"
set ylabel "y"
set size square
plot "heavily_damped.dat" u 1:2 w l
unset output

set output "heavily_damped_phase.pdf"
set title "Heavily damped, phase space"
set xlabel "y"
set ylabel "y'(t)"
set size square
plot "heavily_damped.dat" u 2:3 w l
unset output

set output "critically_damped.pdf"
set title "Critically damped"
set xlabel "t"
set ylabel "y"
set size square
plot "critically_damped.dat" u 1:2 w l
unset output

set output "critically_damped_phase.pdf"
set title "Critically damped, phase space"
set xlabel "y"
set ylabel "y'(t)"
set size square
plot "critically_damped.dat" u 2:3 w l
unset output

set output "resonant_force.pdf"
set title "Forced at resonace"
set xlabel "t"
set ylabel "y"
set size square
plot "resonant_force.dat" u 1:2 w l
unset output

set output "resonant_force_phase.pdf"
set title "Forced at resonace, phase space"
set xlabel "y"
set ylabel "y'(t)"
set size square
plot "resonant_force.dat" u 2:3 w l
unset output

set output "offresonant_force.pdf"
set title "Forced away from resonace"
set xlabel "t"
set ylabel "y"
set size square
plot "offresonant_force.dat" u 1:2 w l
unset output

set output "offresonant_force_phase.pdf"
set title "Forced away from resonace, phase space"
set xlabel "y"
set ylabel "y'(t)"
set size square
plot "offresonant_force.dat" u 2:3 w l
unset output

set output "damped_driven_resonant.pdf"
set title "Damped, driven at resonance"
set xlabel "t"
set ylabel "y"
set size square
plot "damped_driven_resonant.dat" u 1:2 w l
unset output

set output "damped_driven_resonant_phase.pdf"
set title "Damped, driven at resonance, phase space"
set xlabel "t"
set ylabel "y"
set size square
plot "damped_driven_resonant.dat" u 2:3 w l
unset output

set output "damped_driven_offresonant.pdf"
set title "Damped, driven at off-resonance"
set xlabel "t"
set ylabel "y"
set size square
plot "damped_driven_offresonant.dat" u 1:2 w l
unset output

set output "damped_driven_offresonant_phase.pdf"
set title "Damped, driven at off-resonance, phase space"
set xlabel "y"
set ylabel "y'(t)"
set size square
plot "damped_driven_offresonant.dat" u 2:3 w l
unset output

set output "critdamped_driven.pdf"
set title "Critically damped and driven off-resonance"
set xlabel "t"
set ylabel "y"
set size square
plot "critdamped_driven.dat" u 1:2 w l
unset output

set output "critdamped_driven_phase.pdf"
set title "Critically damped and driven off-resonance, phase space"
set xlabel "y"
set ylabel "y'(t)"
set size square
plot "critdamped_driven.dat" u 2:3 w l
unset output

unset terminal