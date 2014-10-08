#include "modules/NeuralNetTTLQ.hh"
ClassImp(NeuralNet)
void NeuralNet::Initialize()
{
// build network structure
fLayers = 3;
fLayerSize[0] = 7; fWeights[0] = new double[7];
fLayerSize[1] = 12; fWeights[1] = new double[12];
fLayerSize[2] = 1; fWeights[2] = new double[1];
// weight matrix from layer 0 to 1
fWeightMatrix0to1[0][0] = -1.84147299832242;
fWeightMatrix0to1[1][0] = 1.88264692855664;
fWeightMatrix0to1[2][0] = 0.529609398104258;
fWeightMatrix0to1[3][0] = 1.48476072951336;
fWeightMatrix0to1[4][0] = -2.00819465158201;
fWeightMatrix0to1[5][0] = -2.10894279632663;
fWeightMatrix0to1[6][0] = -0.689519802605384;
fWeightMatrix0to1[7][0] = 2.16884893893128;
fWeightMatrix0to1[8][0] = -1.31057326449207;
fWeightMatrix0to1[9][0] = -0.826748293773794;
fWeightMatrix0to1[10][0] = -1.46365440004152;
fWeightMatrix0to1[0][1] = -1.63732417034887;
fWeightMatrix0to1[1][1] = -1.04742340585959;
fWeightMatrix0to1[2][1] = -0.328762594784061;
fWeightMatrix0to1[3][1] = -0.566867199821349;
fWeightMatrix0to1[4][1] = 0.670816926255183;
fWeightMatrix0to1[5][1] = -1.76246106182233;
fWeightMatrix0to1[6][1] = 1.26075544969035;
fWeightMatrix0to1[7][1] = 0.567418278963214;
fWeightMatrix0to1[8][1] = 1.61610036037718;
fWeightMatrix0to1[9][1] = -0.290496754722036;
fWeightMatrix0to1[10][1] = -0.419771973208075;
fWeightMatrix0to1[0][2] = -0.856296247411626;
fWeightMatrix0to1[1][2] = 0.476490488520169;
fWeightMatrix0to1[2][2] = -1.31099027988765;
fWeightMatrix0to1[3][2] = -0.0611152401029023;
fWeightMatrix0to1[4][2] = 1.49342440197892;
fWeightMatrix0to1[5][2] = -0.949590865286619;
fWeightMatrix0to1[6][2] = -1.96674398658555;
fWeightMatrix0to1[7][2] = 0.423827167167475;
fWeightMatrix0to1[8][2] = 0.667212830754865;
fWeightMatrix0to1[9][2] = 1.67411942008189;
fWeightMatrix0to1[10][2] = -0.127348978335791;
fWeightMatrix0to1[0][3] = -0.284427255308567;
fWeightMatrix0to1[1][3] = 0.627858842253664;
fWeightMatrix0to1[2][3] = -0.483179148328505;
fWeightMatrix0to1[3][3] = -1.29874728594678;
fWeightMatrix0to1[4][3] = 1.76915594243324;
fWeightMatrix0to1[5][3] = 0.0558379600728344;
fWeightMatrix0to1[6][3] = 0.609377587659296;
fWeightMatrix0to1[7][3] = 1.3942560773756;
fWeightMatrix0to1[8][3] = 0.795596578036156;
fWeightMatrix0to1[9][3] = 2.05691482481512;
fWeightMatrix0to1[10][3] = 0.798570352860225;
fWeightMatrix0to1[0][4] = 0.2832560767856;
fWeightMatrix0to1[1][4] = -1.48158500757226;
fWeightMatrix0to1[2][4] = 0.0437726347625343;
fWeightMatrix0to1[3][4] = -0.213447026987525;
fWeightMatrix0to1[4][4] = 1.45307168255505;
fWeightMatrix0to1[5][4] = 0.375641768576004;
fWeightMatrix0to1[6][4] = 1.9908973799924;
fWeightMatrix0to1[7][4] = 1.29494099030791;
fWeightMatrix0to1[8][4] = -1.38615145277991;
fWeightMatrix0to1[9][4] = 0.972167950645267;
fWeightMatrix0to1[10][4] = -0.52261870329383;
fWeightMatrix0to1[0][5] = -1.27015320570469;
fWeightMatrix0to1[1][5] = -1.20373904806369;
fWeightMatrix0to1[2][5] = -1.60082150423541;
fWeightMatrix0to1[3][5] = 1.37013859843214;
fWeightMatrix0to1[4][5] = 0.729460196709145;
fWeightMatrix0to1[5][5] = -1.34756615504172;
fWeightMatrix0to1[6][5] = -0.0575986253204272;
fWeightMatrix0to1[7][5] = -0.00620912738910997;
fWeightMatrix0to1[8][5] = 0.730752337919072;
fWeightMatrix0to1[9][5] = 1.85852932855562;
fWeightMatrix0to1[10][5] = 0.321541346607982;
fWeightMatrix0to1[0][6] = -3.81241074310542;
fWeightMatrix0to1[1][6] = -1.32113173929044;
fWeightMatrix0to1[2][6] = 1.39293175913992;
fWeightMatrix0to1[3][6] = -0.421833400290414;
fWeightMatrix0to1[4][6] = -0.688754571277557;
fWeightMatrix0to1[5][6] = -3.33244577563278;
fWeightMatrix0to1[6][6] = 0.533842281775873;
fWeightMatrix0to1[7][6] = 0.283481703910216;
fWeightMatrix0to1[8][6] = 1.37544699770734;
fWeightMatrix0to1[9][6] = -0.338241591297261;
fWeightMatrix0to1[10][6] = -2.9980996075799;
// weight matrix from layer 1 to 2
fWeightMatrix1to2[0][0] = -1.2222705365932;
fWeightMatrix1to2[0][1] = -0.0425045074469058;
fWeightMatrix1to2[0][2] = -0.0440113822160976;
fWeightMatrix1to2[0][3] = -0.016551534706383;
fWeightMatrix1to2[0][4] = 0.0190350013213871;
fWeightMatrix1to2[0][5] = 0.6549335579393;
fWeightMatrix1to2[0][6] = -0.0594387612303241;
fWeightMatrix1to2[0][7] = -0.0487147682492331;
fWeightMatrix1to2[0][8] = -0.0652800474562168;
fWeightMatrix1to2[0][9] = 0.0699585050027424;
fWeightMatrix1to2[0][10] = -1.0508318130349;
fWeightMatrix1to2[0][11] = -0.533250362981289;
}

double NeuralNet::GetMvaValue__( const std::vector<double>& inputValues ) const
{
if (inputValues.size() != (unsigned int)fLayerSize[0]-1) {
std::cout << "Input vector needs to be of size " << fLayerSize[0]-1 << std::endl;
return 0;
}

for (int l=0; l<fLayers; l++)
for (int i=0; i<fLayerSize[l]; i++) fWeights[l][i]=0;

for (int l=0; l<fLayers-1; l++)
fWeights[l][fLayerSize[l]-1]=1;

for (int i=0; i<fLayerSize[0]-1; i++)
fWeights[0][i]=inputValues[i];

// layer 0 to 1
for (int o=0; o<fLayerSize[1]-1; o++) {
for (int i=0; i<fLayerSize[0]; i++) {
double inputVal = fWeightMatrix0to1[o][i] * fWeights[0][i];
fWeights[1][o] += inputVal;
}
fWeights[1][o] = ActivationFnc(fWeights[1][o]);
}
// layer 1 to 2
for (int o=0; o<fLayerSize[2]; o++) {
for (int i=0; i<fLayerSize[1]; i++) {
double inputVal = fWeightMatrix1to2[o][i] * fWeights[1][i];
fWeights[2][o] += inputVal;
}
fWeights[2][o] = OutputActivationFnc(fWeights[2][o]);
}

return fWeights[2][0];
}

double NeuralNet::ActivationFnc(double x) const {
// hyperbolic tan
return tanh(x);
}
double NeuralNet::OutputActivationFnc(double x) const {
// identity
return x;
}

// Clean up
void NeuralNet::Clear()
{
// nothing to clear
}
double NeuralNet::GetMvaValue( const std::vector<double>& inputValues ) const
{
// classifier response value
double retval = 0;

// classifier response, sanity check first
if (!IsStatusClean()) {
std::cout << "Problem in class "" << fClassName << "": cannot return classifier response"
<< " because status is dirty" << std::endl;
retval = 0;
}
else {
if (IsNormalised()) {
// normalise variables
std::vector<double> iV;
int ivar = 0;
for (std::vector<double>::const_iterator varIt = inputValues.begin();
varIt != inputValues.end(); varIt++, ivar++) {
iV.push_back(NormVariable( *varIt, fVmin[ivar], fVmax[ivar] ));
}
Transform( iV, -1 );
retval = GetMvaValue__( iV );
}
else {
std::vector<double> iV;
int ivar = 0;
for (std::vector<double>::const_iterator varIt = inputValues.begin();
varIt != inputValues.end(); varIt++, ivar++) {
iV.push_back(*varIt);
}
Transform( iV, -1 );
retval = GetMvaValue__( iV );
}
}

return retval;
}

//_______________________________________________________________________
void NeuralNet::InitTransform_1()
{
// Normalization transformation, initialisation
fMin_1[0][0] = 112.698600769;
fMax_1[0][0] = 1998.47119141;
fMin_1[1][0] = 100.001274109;
fMax_1[1][0] = 958.050842285;
fMin_1[2][0] = 100.001274109;
fMax_1[2][0] = 1998.47119141;
fMin_1[0][1] = 50.7634429932;
fMax_1[0][1] = 1445.94445801;
fMin_1[1][1] = 30.9686775208;
fMax_1[1][1] = 928.628112793;
fMin_1[2][1] = 30.9686775208;
fMax_1[2][1] = 1445.94445801;
fMin_1[0][2] = 4.67570734024;
fMax_1[0][2] = 1020.6953125;
fMin_1[1][2] = 1.48323011398;
fMax_1[1][2] = 398.764892578;
fMin_1[2][2] = 1.48323011398;
fMax_1[2][2] = 1020.6953125;
fMin_1[0][3] = 0.131739616394;
fMax_1[0][3] = 1730.38537598;
fMin_1[1][3] = 0.0126247275621;
fMax_1[1][3] = 415.016418457;
fMin_1[2][3] = 0.0126247275621;
fMax_1[2][3] = 1730.38537598;
fMin_1[0][4] = 8.63075256348e-05;
fMax_1[0][4] = 3.12506818771;
fMin_1[1][4] = 0.00473856925964;
fMax_1[1][4] = 3.13914489746;
fMin_1[2][4] = 8.63075256348e-05;
fMax_1[2][4] = 3.13914489746;
fMin_1[0][5] = 68.0134963989;
fMax_1[0][5] = 1135.73706055;
fMin_1[1][5] = 34.073223114;
fMax_1[1][5] = 685.584228516;
fMin_1[2][5] = 34.073223114;
fMax_1[2][5] = 1135.73706055;
}

//_______________________________________________________________________
void NeuralNet::Transform_1( std::vector<double>& iv, int cls) const
{
// Normalization transformation
if (cls < 0 || cls > 2) {
if (2 > 1 ) cls = 2;
else cls = 2;
}
const int nVar = 6;

// get indices of used variables

// define the indices of the variables which are transformed by this transformation
std::vector<int> indicesGet;
std::vector<int> indicesPut;

indicesGet.push_back( 0);
indicesGet.push_back( 1);
indicesGet.push_back( 2);
indicesGet.push_back( 3);
indicesGet.push_back( 4);
indicesGet.push_back( 5);
indicesPut.push_back( 0);
indicesPut.push_back( 1);
indicesPut.push_back( 2);
indicesPut.push_back( 3);
indicesPut.push_back( 4);
indicesPut.push_back( 5);

std::vector<double> dv(nVar);
for (int ivar=0; ivar<nVar; ivar++) dv[ivar] = iv[indicesGet.at(ivar)];
for (int ivar=0;ivar<6;ivar++) {
double offset = fMin_1[cls][ivar];
double scale  = 1.0/(fMax_1[cls][ivar]-fMin_1[cls][ivar]);
iv[indicesPut.at(ivar)] = (dv[ivar]-offset)*scale * 2 - 1;
}
}

//_______________________________________________________________________
void NeuralNet::InitTransform()
{
InitTransform_1();
}

//_______________________________________________________________________
void NeuralNet::Transform( std::vector<double>& iv, int sigOrBgd ) const
{
Transform_1( iv, sigOrBgd );
}
