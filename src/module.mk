# Source files to compile 
FILES :=TestAnalysis 
FILES +=LQTopAnalysis 
FILES +=LQAnalysis 
FILES +=LQFastAnalysis 
FILES +=TopGenAnalysis 
FILES +=LQAccAnalysis 
FILES +=TauFRAnalysis 
FILES +=ZNormAnalysis 
FILES +=METAnalysis
FILES +=ZTauAnalysis
FILES +=TTEffAnalysis
FILES +=TestUncAnalysis
FILES +=MVAConfigAnalysis

# Header files to use for dictionary generation
DICTFILES := $(FILES) LinkDef
