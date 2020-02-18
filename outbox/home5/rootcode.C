auto pi = TMath::Pi();

TCanvas canvas_1("c", "c");

double single(double *x, double *par){
	return pow(sin(pi*par[0]*x[0])/(pi*par[0]*x[0]),2);
}

double nslit0(double *x, double *par){
	return pow(sin(pi*par[1]*x[0])/(pi*x[0]),2);
}

double nslit(double *x, double *par){
	return single(x, par) * nslit0(x, par);
}

void slits(){
	float r, ns;
	r = 0.2; //slit width
	ns = 2; //num of slits
	
	TF1 *Fnslits = new TF1("Fnslits", nslit, -5.0, 5.0, 2);
	Fnslits->SetNpx(500);

	Fnslits->SetParameter(0, r);
	Fnslits->SetParameter(1, ns);

	Fnslits->Draw();
}

void rootcode(){
	slits();
	canvas_1.Draw();
}
