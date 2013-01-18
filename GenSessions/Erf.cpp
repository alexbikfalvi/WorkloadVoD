#include "stdafx.h"
#include "Erf.h"
#include "Chebyshev.h"

double erfInvSeriesCoeff[1001] = {
	8.8622692545275794e-001,
	2.3201366653465447e-001,
	1.2755617530559790e-001,
	8.6552129241547496e-002,
	6.4959617745385362e-002,
	5.1731281984616323e-002,
	4.2836720651797305e-002,
	3.6465929308531576e-002,
	3.1689005021605390e-002,
	2.7980632964995176e-002,
	2.5022275841198299e-002,
	2.2609863318897524e-002,
	2.0606780379058956e-002,
	1.8918217250778815e-002,
	1.7476370562856499e-002,
	1.6231500987685204e-002,
	1.5146315063247762e-002,
	1.4192316002509918e-002,
	1.3347364197421252e-002,
	1.2594004871332025e-002,
	1.1918295936391992e-002,
	1.1308970105922495e-002,
	1.0756825303317914e-002,
	1.0254274081853426e-002,
	9.7950057700711361e-003,
	9.3737298191820407e-003,
	8.9859785028433403e-003,
	8.6279535807093945e-003,
	8.2964059277391995e-003,
	7.9885401626033168e-003,
	7.7019384322597265e-003,
	7.4344990178314948e-003,
	7.1843865112682700e-003,
	6.9499911010646722e-003,
	6.7298950853414887e-003,
	6.5228451614500211e-003,
	6.3277293643431041e-003,
	6.1435577703841168e-003,
	5.9694462697344186e-003,
	5.8046028538342610e-003,
	5.6483159755515225e-003,
	5.4999446262039269e-003,
	5.3589098416864066e-003,
	5.2246874036874493e-003,
	5.0968015447061645e-003,
	4.9748194997389994e-003,
	4.8583467749584450e-003,
	4.7470230258849352e-003,
	4.6405184555589904e-003,
	4.5385306579070464e-003,
	4.4407818435271435e-003,
	4.3470163950214708e-003,
	4.2569987071826906e-003,
	4.1705112741261333e-003,
	4.0873529911089838e-003,
	4.0073376434981035e-003,
	3.9302925593064407e-003,
	3.8560574050481807e-003,
	3.7844831074737911e-003,
	3.7154308861260124e-003,
	3.6487713836790584e-003,
	3.5843838827445333e-003,
	3.5221555992978322e-003,
	3.4619810441376213e-003,
	3.4037614448720341e-003,
	3.3474042218555292e-003,
	3.2928225123032809e-003,
	3.2399347375041952e-003,
	3.1886642086555931e-003,
	3.1389387673655391e-003,
	3.0906904573240065e-003,
	3.0438552240413226e-003,
	2.9983726398995778e-003,
	2.9541856520668105e-003,
	2.9112403510907882e-003,
	2.8694857582237973e-003,
	2.8288736297367003e-003,
	2.7893582766628159e-003,
	2.7508963985734881e-003,
	2.7134469301298035e-003,
	2.6769708992816642e-003,
	2.6414312960976682e-003,
	2.6067929513092763e-003,
	2.5730224237419069e-003,
	2.5400878958848831e-003,
	2.5079590769232969e-003,
	2.4766071126182732e-003,
	2.4460045014790635e-003,
	2.4161250167213421e-003,
	2.3869436335520219e-003,
	2.3584364613620261e-003,
	2.3305806804456075e-003,
	2.3033544828982627e-003,
	2.2767370173754678e-003,
	2.2507083374217205e-003,
	2.2252493531041118e-003,
	2.2003417857069277e-003,
	2.1759681252640716e-003,
	2.1521115907244997e-003,
	2.1287560925625083e-003,
	2.1058861976599309e-003,
	2.0834870963011491e-003,
	2.0615445711343336e-003,
	2.0400449679639121e-003,
	2.0189751682496565e-003,
	1.9983225631974352e-003,
	1.9780750293353641e-003,
	1.9582209054771465e-003,
	1.9387489709817420e-003,
	1.9196484252251909e-003,
	1.9009088682066374e-003,
	1.8825202822162538e-003,
	1.8644730144979718e-003,
	1.8467577608447252e-003,
	1.8293655500683313e-003,
	1.8122877292901737e-003,
	1.7955159500026427e-003,
	1.7790421548546859e-003,
	1.7628585651180597e-003,
	1.7469576687938015e-003,
	1.7313322093211526e-003,
	1.7159751748537233e-003,
	1.7008797880699519e-003,
	1.6860394964871670e-003,
	1.6714479632504707e-003,
	1.6570990583695609e-003,
	1.6429868503783638e-003,
	1.6291055983938840e-003,
	1.6154497445521887e-003,
	1.6020139068008800e-003,
	1.5887928720285918e-003,
	1.5757815895133392e-003,
	1.5629751646726092e-003,
	1.5503688530991353e-003,
	1.5379580548672601e-003,
	1.5257383090957121e-003,
	1.5137052887534382e-003,
	1.5018547956959381e-003,
	1.4901827559202778e-003,
	1.4786852150276545e-003,
	1.4673583338829944e-003,
	1.4561983844617122e-003,
	1.4452017458743149e-003,
	1.4343649005599921e-003,
	1.4236844306409623e-003,
	1.4131570144296486e-003,
	1.4027794230813421e-003,
	1.3925485173852969e-003,
	1.3824612446876583e-003,
	1.3725146359399594e-003,
	1.3627058028672550e-003,
	1.3530319352502806e-003,
	1.3434902983163449e-003,
	1.3340782302338889e-003,
	1.3247931397059891e-003,
	1.3156325036582420e-003,
	1.3065938650167949e-003,
	1.2976748305724053e-003,
	1.2888730689267142e-003,
	1.2801863085170585e-003,
	1.2716123357163444e-003,
	1.2631489930046845e-003,
	1.2547941772096595e-003,
	1.2465458378122306e-003,
	1.2384019753154617e-003,
	1.2303606396733625e-003,
	1.2224199287772747e-003,
	1.2145779869973767e-003,
	1.2068330037769872e-003,
	1.1991832122774269e-003,
	1.1916268880713746e-003,
	1.1841623478826713e-003,
	1.1767879483706859e-003,
	1.1695020849574070e-003,
	1.1623031906955249e-003,
	1.1551897351758473e-003,
	1.1481602234724602e-003,
	1.1412131951241296e-003,
	1.1343472231505076e-003,
	1.1275609131017504e-003,
	1.1208529021402487e-003,
	1.1142218581532074e-003,
	1.1076664788948773e-003,
	1.1011854911572828e-003,
	1.0947776499683724e-003,
	1.0884417378165066e-003,
	1.0821765639003125e-003,
	1.0759809634029313e-003,
	1.0698537967897377e-003,
	1.0637939491286599e-003,
	1.0578003294322432e-003,
	1.0518718700206764e-003,
	1.0460075259049765e-003,
	1.0402062741896229e-003,
	1.0344671134939062e-003,
	1.0287890633913250e-003,
	1.0231711638663790e-003,
	1.0176124747881289e-003,
	1.0121120753999234e-003,
	1.0066690638247193e-003,
	1.0012825565854434e-003,
	9.9595168813986727e-004,
	9.9067561042948614e-004,
	9.8545349244190026e-004,
	9.8028451978626014e-004,
	9.7516789428128876e-004,
	9.7010283355547532e-004,
	9.6508857065901235e-004,
	9.6012435368707650e-004,
	9.5520944541407189e-004,
	9.5034312293846819e-004,
	9.4552467733786960e-004,
	9.4075341333397654e-004,
	9.3602864896712222e-004,
	9.3134971528005376e-004,
	9.2671595601064635e-004,
	9.2212672729328820e-004,
	9.1758139736862010e-004,
	9.1307934630137485e-004,
	9.0861996570605228e-004,
	9.0420265848018325e-004,
	8.9982683854491997e-004,
	8.9549193059274381e-004,
	8.9119736984205134e-004,
	8.8694260179839556e-004,
	8.8272708202219224e-004,
	8.7855027590266212e-004,
	8.7441165843784934e-004,
	8.7031071402048272e-004,
	8.6624693622953930e-004,
	8.6221982762732063e-004,
	8.5822889956186027e-004,
	8.5427367197452194e-004,
	8.5035367321260869e-004,
	8.4646843984684847e-004,
	8.4261751649360303e-004,
	8.3880045564165863e-004,
	8.3501681748344850e-004,
	8.3126616975061232e-004,
	8.2754808755371775e-004,
	8.2386215322604857e-004,
	8.2020795617134359e-004,
	8.1658509271535285e-004,
	8.1299316596110903e-004,
	8.0943178564780874e-004,
	8.0590056801319834e-004,
	8.0239913565935351e-004,
	7.9892711742177549e-004,
	7.9548414824168353e-004,
	7.9206986904143978e-004,
	7.8868392660299169e-004,
	7.8532597344927000e-004,
	7.8199566772844205e-004,
	7.7869267310095257e-004,
	7.7541665862925987e-004,
	7.7216729867021262e-004,
	7.6894427276997721e-004,
	7.6574726556145054e-004,
	7.6257596666409397e-004,
	7.5943007058611460e-004,
	7.5630927662894104e-004,
	7.5321328879392028e-004,
	7.5014181569117949e-004,
	7.4709457045059541e-004,
	7.4407127063482131e-004,
	7.4107163815429627e-004,
	7.3809539918421110e-004,
	7.3514228408335801e-004,
	7.3221202731482368e-004,
	7.2930436736847110e-004,
	7.2641904668517576e-004,
	7.2355581158275398e-004,
	7.2071441218355314e-004,
	7.1789460234364768e-004,
	7.1509613958361487e-004,
	7.1231878502083261e-004,
	7.0956230330327904e-004,
	7.0682646254477605e-004,
	7.0411103426165286e-004,
	7.0141579331079503e-004,
	6.9874051782902596e-004,
	6.9608498917381472e-004,
	6.9344899186525371e-004,
	6.9083231352928993e-004,
	6.8823474484215824e-004,
	6.8565607947601646e-004,
	6.8309611404572957e-004,
	6.8055464805677631e-004,
	6.7803148385426086e-004,
	6.7552642657300096e-004,
	6.7303928408865587e-004,
	6.7056986696988053e-004,
	6.6811798843146913e-004,
	6.6568346428848063e-004,
	6.6326611291130599e-004,
	6.6086575518166444e-004,
	6.5848221444950324e-004,
	6.5611531649078006e-004,
	6.5376488946610584e-004,
	6.5143076388022984e-004,
	6.4911277254234702e-004,
	6.4681075052720283e-004,
	6.4452453513698708e-004,
	6.4225396586398901e-004,
	6.3999888435399695e-004,
	6.3775913437043611e-004,
	6.3553456175920891e-004,
	6.3332501441423880e-004,
	6.3113034224368991e-004,
	6.2895039713685473e-004,
	6.2678503293169213e-004,
	6.2463410538299237e-004,
	6.2249747213117526e-004,
	6.2037499267168165e-004,
	6.1826652832496778e-004,
	6.1617194220706659e-004,
	6.1409109920073453e-004,
	6.1202386592713192e-004,
	6.0997011071805727e-004,
	6.0792970358870780e-004,
	6.0590251621095296e-004,
	6.0388842188712291e-004,
	6.0188729552428251e-004,
	5.9989901360899857e-004,
	5.9792345418257565e-004,
	5.9596049681675265e-004,
	5.9401002258985833e-004,
	5.9207191406340928e-004,
	5.9014605525913662e-004,
	5.8823233163644449e-004,
	5.8633063007027713e-004,
	5.8444083882939959e-004,
	5.8256284755506867e-004,
	5.8069654724010025e-004,
	5.7884183020831136e-004,
	5.7699859009434059e-004,
	5.7516672182383042e-004,
	5.7334612159396967e-004,
	5.7153668685438423e-004,
	5.6973831628836851e-004,
	5.6795090979445929e-004,
	5.6617436846832932e-004,
	5.6440859458501097e-004,
	5.6265349158142832e-004,
	5.6090896403924431e-004,
	5.5917491766800350e-004,
	5.5745125928857557e-004,
	5.5573789681688990e-004,
	5.5403473924794781e-004,
	5.5234169664012322e-004,
	5.5065868009972587e-004,
	5.4898560176583930e-004,
	5.4732237479541675e-004,
	5.4566891334863698e-004,
	5.4402513257450808e-004,
	5.4239094859671909e-004,
	5.4076627849973611e-004,
	5.3915104031513012e-004,
	5.3754515300814094e-004,
	5.3594853646446829e-004,
	5.3436111147728413e-004,
	5.3278279973446331e-004,
	5.3121352380603381e-004,
	5.2965320713183105e-004,
	5.2810177400935867e-004,
	5.2655914958186047e-004,
	5.2502525982657663e-004,
	5.2350003154320553e-004,
	5.2198339234255174e-004,
	5.2047527063535999e-004,
	5.1897559562133781e-004,
	5.1748429727834985e-004,
	5.1600130635179797e-004,
	5.1452655434416623e-004,
	5.1305997350474216e-004,
	5.1160149681949991e-004,
	5.1015105800115122e-004,
	5.0870859147935418e-004,
	5.0727403239107869e-004,
	5.0584731657113077e-004,
	5.0442838054282058e-004,
	5.0301716150879009e-004,
	5.0161359734197779e-004,
	5.0021762657673143e-004,
	4.9882918840005641e-004,
	4.9744822264301237e-004,
	4.9607466977223420e-004,
	4.9470847088159148e-004,
	4.9334956768398177e-004,
	4.9199790250324031e-004,
	4.9065341826619552e-004,
	4.8931605849482796e-004,
	4.8798576729856163e-004,
	4.8666248936667344e-004,
	4.8534616996081967e-004,
	4.8403675490767367e-004,
	4.8273419059168275e-004,
	4.8143842394792982e-004,
	4.8014940245510844e-004,
	4.7886707412860360e-004,
	4.7759138751367661e-004,
	4.7632229167875661e-004,
	4.7505973620882893e-004,
	4.7380367119892572e-004,
	4.7255404724771304e-004,
	4.7131081545117909e-004,
	4.7007392739641037e-004,
	4.6884333515546500e-004,
	4.6761899127933503e-004,
	4.6640084879200072e-004,
	4.6518886118457141e-004,
	4.6398298240951164e-004,
	4.6278316687495697e-004,
	4.6158936943911064e-004,
	4.6040154540472067e-004,
	4.5921965051364195e-004,
	4.5804364094147306e-004,
	4.5687347329227657e-004,
	4.5570910459337302e-004,
	4.5455049229020943e-004,
	4.5339759424130473e-004,
	4.5225036871326729e-004,
	4.5110877437588354e-004,
	4.4997277029727766e-004,
	4.4884231593913886e-004,
	4.4771737115202181e-004,
	4.4659789617070857e-004,
	4.4548385160964271e-004,
	4.4437519845841843e-004,
	4.4327189807734347e-004,
	4.4217391219306351e-004,
	4.4108120289423940e-004,
	4.3999373262729776e-004,
	4.3891146419223088e-004,
	4.3783436073846141e-004,
	4.3676238576076403e-004,
	4.3569550309524075e-004,
	4.3463367691535658e-004,
	4.3357687172802648e-004,
	4.3252505236976045e-004,
	4.3147818400285934e-004,
	4.3043623211165701e-004,
	4.2939916249883021e-004,
	4.2836694128174435e-004,
	4.2733953488885434e-004,
	4.2631691005615488e-004,
	4.2529903382367912e-004,
	4.2428587353204731e-004,
	4.2327739681905462e-004,
	4.2227357161631253e-004,
	4.2127436614593678e-004,
	4.2027974891726974e-004,
	4.1928968872365844e-004,
	4.1830415463927029e-004,
	4.1732311601594949e-004,
	4.1634654248012332e-004,
	4.1537440392973768e-004,
	4.1440667053124417e-004,
	4.1344331271662651e-004,
	4.1248430118045375e-004,
	4.1152960687699055e-004,
	4.1057920101733239e-004,
	4.0963305506658217e-004,
	4.0869114074106532e-004,
	4.0775343000558340e-004,
	4.0681989507069703e-004,
	4.0589050839004768e-004,
	4.0496524265771881e-004,
	4.0404407080562059e-004,
	4.0312696600092190e-004,
	4.0221390164350520e-004,
	4.0130485136345711e-004,
	4.0039978901859451e-004,
	3.9949868869201518e-004,
	3.9860152468968889e-004,
	3.9770827153807526e-004,
	3.9681890398176733e-004,
	3.9593339698117309e-004,
	3.9505172571022314e-004,
	3.9417386555410354e-004,
	3.9329979210702711e-004,
	3.9242948117001927e-004,
	3.9156290874874637e-004,
	3.9070005105136014e-004,
	3.8984088448637875e-004,
	3.8898538566057921e-004,
	3.8813353137693538e-004,
	3.8728529863256943e-004,
	3.8644066461672905e-004,
	3.8559960670879635e-004,
	3.8476210247631826e-004,
	3.8392812967305323e-004,
	3.8309766623705911e-004,
	3.8227069028879117e-004,
	3.8144718012922445e-004,
	3.8062711423800531e-004,
	3.7981047127162324e-004,
	3.7899723006160063e-004,
	3.7818736961271206e-004,
	3.7738086910121837e-004,
	3.7657770787312834e-004,
	3.7577786544247364e-004,
	3.7498132148961491e-004,
	3.7418805585955902e-004,
	3.7339804856030236e-004,
	3.7261127976119106e-004,
	3.7182772979130256e-004,
	3.7104737913784487e-004,
	3.7027020844457873e-004,
	3.6949619851025351e-004,
	3.6872533028706482e-004,
	3.6795758487912942e-004,
	3.6719294354098072e-004,
	3.6643138767607805e-004,
	3.6567289883533656e-004,
	3.6491745871567181e-004,
	3.6416504915856725e-004,
	3.6341565214864993e-004,
	3.6266924981228867e-004,
	3.6192582441621067e-004,
	3.6118535836612722e-004,
	3.6044783420538073e-004,
	3.5971323461360578e-004,
	3.5898154240540772e-004,
	3.5825274052905023e-004,
	3.5752681206517053e-004,
	3.5680374022549428e-004,
	3.5608350835157631e-004,
	3.5536609991355185e-004,
	3.5465149850890136e-004,
	3.5393968786123320e-004,
	3.5323065181907349e-004,
	3.5252437435467931e-004,
	3.5182083956285400e-004,
	3.5112003165978884e-004,
	3.5042193498190457e-004,
	3.4972653398471817e-004,
	3.4903381324171231e-004,
	3.4834375744322520e-004,
	3.4765635139534929e-004,
	3.4697158001884243e-004,
	3.4628942834805039e-004,
	3.4560988152984774e-004,
	3.4493292482257930e-004,
	3.4425854359502474e-004,
	3.4358672332536710e-004,
	3.4291744960017793e-004,
	3.4225070811340911e-004,
	3.4158648466539875e-004,
	3.4092476516188805e-004,
	3.4026553561304884e-004,
	3.3960878213251813e-004,
	3.3895449093645054e-004,
	3.3830264834257490e-004,
	3.3765324076926351e-004,
	3.3700625473461312e-004,
	3.3636167685553183e-004,
	3.3571949384684157e-004,
	3.3507969252038490e-004,
	3.3444225978414429e-004,
	3.3380718264137011e-004,
	3.3317444818972171e-004,
	3.3254404362040963e-004,
	3.3191595621735612e-004,
	3.3129017335635865e-004,
	3.3066668250426488e-004,
	3.3004547121815591e-004,
	3.2942652714453848e-004,
	3.2880983801854654e-004,
	3.2819539166314754e-004,
	3.2758317598836532e-004,
	3.2697317899049987e-004,
	3.2636538875136705e-004,
	3.2575979343753812e-004,
	3.2515638129959081e-004,
	3.2455514067136738e-004,
	3.2395605996924173e-004,
	3.2335912769139137e-004,
	3.2276433241708086e-004,
	3.2217166280595059e-004,
	3.2158110759731234e-004,
	3.2099265560945435e-004,
	3.2040629573895071e-004,
	3.1982201695998131e-004,
	3.1923980832365733e-004,
	3.1865965895735147e-004,
	3.1808155806404127e-004,
	3.1750549492165053e-004,
	3.1693145888240647e-004,
	3.1635943937219504e-004,
	3.1578942588993413e-004,
	3.1522140800693696e-004,
	3.1465537536629979e-004,
	3.1409131768228056e-004,
	3.1352922473969609e-004,
	3.1296908639331765e-004,
	3.1241089256727513e-004,
	3.1185463325446788e-004,
	3.1130029851598036e-004,
	3.1074787848050476e-004,
	3.1019736334376860e-004,
	3.0964874336796938e-004,
	3.0910200888121343e-004,
	3.0855715027696095e-004,
	3.0801415801347509e-004,
	3.0747302261328543e-004,
	3.0693373466263824e-004,
	3.0639628481097365e-004,
	3.0586066377039182e-004,
	3.0532686231513539e-004,
	3.0479487128106704e-004,
	3.0426468156516271e-004,
	3.0373628412500069e-004,
	3.0320966997826317e-004,
	3.0268483020223709e-004,
	3.0216175593332289e-004,
	3.0164043836654650e-004,
	3.0112086875507834e-004,
	3.0060303840975545e-004,
	3.0008693869860954e-004,
	2.9957256104639501e-004,
	2.9905989693412957e-004,
	2.9854893789863375e-004,
	2.9803967553207347e-004,
	2.9753210148151507e-004,
	2.9702620744847462e-004,
	2.9652198518848055e-004,
	2.9601942651063103e-004,
	2.9551852327716719e-004,
	2.9501926740303980e-004,
	2.9452165085548638e-004,
	2.9402566565360949e-004,
	2.9353130386796245e-004,
	2.9303855762013466e-004,
	2.9254741908234190e-004,
	2.9205788047702617e-004,
	2.9156993407645072e-004,
	2.9108357220230353e-004,
	2.9059878722530668e-004,
	2.9011557156482268e-004,
	2.8963391768847276e-004,
	2.8915381811175391e-004,
	2.8867526539765807e-004,
	2.8819825215629864e-004,
	2.8772277104453998e-004,
	2.8724881476562778e-004,
	2.8677637606882667e-004,
	2.8630544774905652e-004,
	2.8583602264653708e-004,
	2.8536809364643483e-004,
	2.8490165367850580e-004,
	2.8443669571675618e-004,
	2.8397321277909250e-004,
	2.8351119792698206e-004,
	2.8305064426511296e-004,
	2.8259154494106206e-004,
	2.8213389314496457e-004,
	2.8167768210917782e-004,
	2.8122290510796316e-004,
	2.8076955545716013e-004,
	2.8031762651386589e-004,
	2.7986711167612082e-004,
	2.7941800438259230e-004,
	2.7897029811226628e-004,
	2.7852398638413521e-004,
	2.7807906275689715e-004,
	2.7763552082864847e-004,
	2.7719335423658886e-004,
	2.7675255665672159e-004,
	2.7631312180355827e-004,
	2.7587504342982909e-004,
	2.7543831532619346e-004,
	2.7500293132095188e-004,
	2.7456888527976480e-004,
	2.7413617110536796e-004,
	2.7370478273729609e-004,
	2.7327471415160672e-004,
	2.7284595936060503e-004,
	2.7241851241257176e-004,
	2.7199236739149785e-004,
	2.7156751841681453e-004,
	2.7114395964312918e-004,
	2.7072168525996615e-004,
	2.7030068949150568e-004,
	2.6988096659632610e-004,
	2.6946251086715033e-004,
	2.6904531663059282e-004,
	2.6862937824690872e-004,
	2.6821469010974546e-004,
	2.6780124664589757e-004,
	2.6738904231506300e-004,
	2.6697807160960108e-004,
	2.6656832905429294e-004,
	2.6615980920610406e-004,
	2.6575250665394819e-004,
	2.6534641601845619e-004,
	2.6494153195174230e-004,
	2.6453784913717634e-004,
	2.6413536228915525e-004,
	2.6373406615287968e-004,
	2.6333395550413006e-004,
	2.6293502514904187e-004,
	2.6253726992389213e-004,
	2.6214068469487768e-004,
	2.6174526435790039e-004,
	2.6135100383835303e-004,
	2.6095789809090826e-004,
	2.6056594209930735e-004,
	2.6017513087615309e-004,
	2.5978545946270194e-004,
	2.5939692292865907e-004,
	2.5900951637197717e-004,
	2.5862323491865385e-004,
	2.5823807372253029e-004,
	2.5785402796509754e-004,
	2.5747109285529583e-004,
	2.5708926362932215e-004,
	2.5670853555043811e-004,
	2.5632890390877780e-004,
	2.5595036402115660e-004,
	2.5557291123088488e-004,
	2.5519654090758230e-004,
	2.5482124844699010e-004,
	2.5444702927079042e-004,
	2.5407387882642272e-004,
	2.5370179258690524e-004,
	2.5333076605065365e-004,
	2.5296079474130552e-004,
	2.5259187420754585e-004,
	2.5222400002292866e-004,
	2.5185716778570820e-004,
	2.5149137311866477e-004,
	2.5112661166893562e-004,
	2.5076287910784730e-004,
	2.5040017113074750e-004,
	2.5003848345683702e-004,
	2.4967781182900893e-004,
	2.4931815201368539e-004,
	2.4895949980065197e-004,
	2.4860185100290073e-004,
	2.4824520145647045e-004,
	2.4788954702028687e-004,
	2.4753488357600944e-004,
	2.4718120702787229e-004,
	2.4682851330253393e-004,
	2.4647679834892221e-004,
	2.4612605813808164e-004,
	2.4577628866302758e-004,
	2.4542748593859296e-004,
	2.4507964600128257e-004,
	2.4473276490912514e-004,
	2.4438683874152944e-004,
	2.4404186359913712e-004,
	2.4369783560368482e-004,
	2.4335475089785574e-004,
	2.4301260564514414e-004,
	2.4267139602971339e-004,
	2.4233111825625701e-004,
	2.4199176854986280e-004,
	2.4165334315587425e-004,
	2.4131583833975691e-004,
	2.4097925038696469e-004,
	2.4064357560280573e-004,
	2.4030881031230982e-004,
	2.3997495086009788e-004,
	2.3964199361025404e-004,
	2.3930993494619240e-004,
	2.3897877127053362e-004,
	2.3864849900497587e-004,
	2.3831911459016851e-004,
	2.3799061448558719e-004,
	2.3766299516941221e-004,
	2.3733625313840243e-004,
	2.3701038490777542e-004,
	2.3668538701108593e-004,
	2.3636125600010470e-004,
	2.3603798844470172e-004,
	2.3571558093272586e-004,
	2.3539403006988933e-004,
	2.3507333247964731e-004,
	2.3475348480309047e-004,
	2.3443448369882164e-004,
	2.3411632584284753e-004,
	2.3379900792846369e-004,
	2.3348252666614358e-004,
	2.3316687878342877e-004,
	2.3285206102481511e-004,
	2.3253807015164669e-004,
	2.3222490294200668e-004,
	2.3191255619060702e-004,
	2.3160102670868605e-004,
	2.3129031132390092e-004,
	2.3098040688021863e-004,
	2.3067131023781729e-004,
	2.3036301827297954e-004,
	2.3005552787798932e-004,
	2.2974883596103165e-004,
	2.2944293944609031e-004,
	2.2913783527284666e-004,
	2.2883352039658215e-004,
	2.2852999178807804e-004,
	2.2822724643351575e-004,
	2.2792528133438171e-004,
	2.2762409350737136e-004,
	2.2732367998428945e-004,
	2.2702403781195863e-004,
	2.2672516405212144e-004,
	2.2642705578134994e-004,
	2.2612971009094998e-004,
	2.2583312408687098e-004,
	2.2553729488961212e-004,
	2.2524221963413202e-004,
	2.2494789546975848e-004,
	2.2465431956010022e-004,
	2.2436148908295463e-004,
	2.2406940123022214e-004,
	2.2377805320781763e-004,
	2.2348744223558392e-004,
	2.2319756554720459e-004,
	2.2290842039011789e-004,
	2.2262000402543315e-004,
	2.2233231372784508e-004,
	2.2204534678554991e-004,
	2.2175910050016248e-004,
	2.2147357218663471e-004,
	2.2118875917317098e-004,
	2.2090465880114903e-004,
	2.2062126842503830e-004,
	2.2033858541231987e-004,
	2.2005660714340526e-004,
	2.1977533101156038e-004,
	2.1949475442282557e-004,
	2.1921487479593631e-004,
	2.1893568956224784e-004,
	2.1865719616565705e-004,
	2.1837939206252797e-004,
	2.1810227472161538e-004,
	2.1782584162398784e-004,
	2.1755009026295453e-004,
	2.1727501814399318e-004,
	2.1700062278467300e-004,
	2.1672690171458561e-004,
	2.1645385247526748e-004,
	2.1618147262013294e-004,
	2.1590975971440038e-004,
	2.1563871133502193e-004,
	2.1536832507061232e-004,
	2.1509859852138156e-004,
	2.1482952929906087e-004,
	2.1456111502683899e-004,
	2.1429335333929050e-004,
	2.1402624188230886e-004,
	2.1375977831303847e-004,
	2.1349396029980949e-004,
	2.1322878552206882e-004,
	2.1296425167031705e-004,
	2.1270035644603967e-004,
	2.1243709756164564e-004,
	2.1217447274039924e-004,
	2.1191247971635757e-004,
	2.1165111623430889e-004,
	2.1139038004970454e-004,
	2.1113026892860133e-004,
	2.1087078064759620e-004,
	2.1061191299376491e-004,
	2.1035366376460118e-004,
	2.1009603076795316e-004,
	2.0983901182196782e-004,
	2.0958260475502496e-004,
	2.0932680740568254e-004,
	2.0907161762261339e-004,
	2.0881703326454973e-004,
	2.0856305220022059e-004,
	2.0830967230829777e-004,
	2.0805689147733599e-004,
	2.0780470760571506e-004,
	2.0755311860158414e-004,
	2.0730212238280555e-004,
	2.0705171687689753e-004,
	2.0680190002098022e-004,
	2.0655266976171683e-004,
	2.0630402405526121e-004,
	2.0605596086720524e-004,
	2.0580847817252033e-004,
	2.0556157395550627e-004,
	2.0531524620973692e-004,
	2.0506949293800787e-004,
	2.0482431215228106e-004,
	2.0457970187363677e-004,
	2.0433566013221730e-004,
	2.0409218496717879e-004,
	2.0384927442663816e-004,
	2.0360692656762149e-004,
	2.0336513945601404e-004,
	2.0312391116651248e-004,
	2.0288323978256888e-004,
	2.0264312339634752e-004,
	2.0240356010867231e-004,
	2.0216454802897939e-004,
	2.0192608527526567e-004,
	2.0168816997404493e-004,
	2.0145080026029601e-004,
	2.0121397427741653e-004,
	2.0097769017717639e-004,
	2.0074194611966964e-004,
	2.0050674027326937e-004,
	2.0027207081457912e-004,
	2.0003793592838882e-004,
	1.9980433380762722e-004,
	1.9957126265331821e-004,
	1.9933872067453559e-004,
	1.9910670608835627e-004,
	1.9887521711981775e-004,
	1.9864425200187389e-004,
	1.9841380897534923e-004,
	1.9818388628889671e-004,
	1.9795448219895378e-004,
	1.9772559496970116e-004,
	1.9749722287301603e-004,
	1.9726936418843372e-004,
	1.9704201720310323e-004,
	1.9681518021174608e-004,
	1.9658885151661355e-004,
	1.9636302942744669e-004,
	1.9613771226143447e-004,
	1.9591289834317293e-004,
	1.9568858600462608e-004,
	1.9546477358508253e-004,
	1.9524145943111777e-004,
	1.9501864189655298e-004,
	1.9479631934241746e-004,
	1.9457449013690807e-004,
	1.9435315265534992e-004,
	1.9413230528015770e-004,
	1.9391194640079791e-004,
	1.9369207441375042e-004,
	1.9347268772246942e-004,
	1.9325378473734710e-004,
	1.9303536387567548e-004,
	1.9281742356160863e-004,
	1.9259996222612604e-004,
	1.9238297830699781e-004,
	1.9216647024874293e-004,
	1.9195043650259885e-004,
	1.9173487552648157e-004,
	1.9151978578495150e-004,
	1.9130516574917652e-004,
	1.9109101389689820e-004,
	1.9087732871239538e-004,
	1.9066410868644903e-004,
	1.9045135231630949e-004,
	1.9023905810565956e-004,
	1.9002722456458123e-004,
	1.8981585020952198e-004,
	1.8960493356326080e-004,
	1.8939447315487404e-004,
	1.8918446751970184e-004,
	1.8897491519931616e-004,
	1.8876581474148684e-004,
	1.8855716470014846e-004,
	1.8834896363536833e-004,
	1.8814121011331508e-004,
	1.8793390270622454e-004,
	1.8772703999236868e-004,
	1.8752062055602369e-004,
	1.8731464298743947e-004,
	1.8710910588280684e-004,
	1.8690400784422638e-004,
	1.8669934747967928e-004,
	1.8649512340299397e-004,
	1.8629133423381795e-004,
	1.8608797859758535e-004,
	1.8588505512548846e-004,
	1.8568256245444608e-004,
	1.8548049922707471e-004,
	1.8527886409165989e-004,
	1.8507765570212353e-004,
	1.8487687271799840e-004,
	1.8467651380439629e-004,
	1.8447657763198046e-004,
	1.8427706287693640e-004,
	1.8407796822094373e-004,
	1.8387929235114722e-004,
	1.8368103396012960e-004,
	1.8348319174588164e-004,
	1.8328576441177570e-004,
	1.8308875066653829e-004,
	1.8289214922422278e-004,
	1.8269595880418009e-004,
	1.8250017813103232e-004,
	1.8230480593464784e-004,
	1.8210984095011181e-004,
	1.8191528191770031e-004,
	1.8172112758285444e-004,
	1.8152737669615285e-004,
	1.8133402801328774e-004,
	1.8114108029503546e-004,
	1.8094853230723386e-004,
	1.8075638282075333e-004,
	1.8056463061147518e-004,
	1.8037327446026317e-004
};

double Erf(double x)
{
	if(fabs(x) < 1.0) return ErfSeries(x);
	else return 1.0 - Erfc(x);
}

double ErfSeries(double x)
{
	double y = x;
	double p = x;
	for(unsigned int i = 1; i < 100; i++)
	{
		p *= -x*x/i;
		y += p / (2*i+1);
	}
	return 1.128379167095513 * y;
}

/*
 * Chebyshev fit for erfc((t+1)/2), -1 < t < 1
 */
static double erfcLess1Data[20] = {
	1.06073416421769980345174155056,
	-0.42582445804381043569204735291,
	0.04955262679620434040357683080,
	0.00449293488768382749558001242,
	-0.00129194104658496953494224761,
	-0.00001836389292149396270416979,
	0.00002211114704099526291538556,
	-5.23337485234257134673693179020e-7,
	-2.78184788833537885382530989578e-7,
	1.41158092748813114560316684249e-8,
	2.72571296330561699984539141865e-9,
	-2.06343904872070629406401492476e-10,
	-2.14273991996785367924201401812e-11,
	2.22990255539358204580285098119e-12,
	1.36250074650698280575807934155e-13,
	-1.95144010922293091898995913038e-14,
	-6.85627169231704599442806370690e-16,
	1.44506492869699938239521607493e-16,
	2.45935306460536488037576200030e-18,
	-9.29599561220523396007359328540e-19
};

SChebyshevSeries erfcLess1Cs = {
	erfcLess1Data,
	19,
	-1, 1,
	12
};

/*
 * Chebyshev fit for erfc(x) exp(x^2), 1 < x < 5, x = 2t + 3, -1 < t < 1
 */
static double erfcLess5Data[25] = {
	0.44045832024338111077637466616,
	-0.143958836762168335790826895326,
	0.044786499817939267247056666937,
	-0.013343124200271211203618353102,
	0.003824682739750469767692372556,
	-0.001058699227195126547306482530,
	0.000283859419210073742736310108,
	-0.000073906170662206760483959432,
	0.000018725312521489179015872934,
	-4.62530981164919445131297264430e-6,
	1.11558657244432857487884006422e-6,
	-2.63098662650834130067808832725e-7,
	6.07462122724551777372119408710e-8,
	-1.37460865539865444777251011793e-8,
	3.05157051905475145520096717210e-9,
	-6.65174789720310713757307724790e-10,
	1.42483346273207784489792999706e-10,
	-3.00141127395323902092018744545e-11,
	6.22171792645348091472914001250e-12,
	-1.26994639225668496876152836555e-12,
	2.55385883033257575402681845385e-13,
	-5.06258237507038698392265499770e-14,
	9.89705409478327321641264227110e-15,
	-1.90685978789192181051961024995e-15,
	3.50826648032737849245113757340e-16
};
SChebyshevSeries erfcLess5Cs = {
	erfcLess5Data,
	24,
	-1, 1,
	16
};

/*
 * Chebyshev fit for erfc(x) x exp(x^2), 5 < x < 10, x = (5t + 15)/2, -1 < t < 1
 */
static double erfcLess10Data[20] = {
	1.11684990123545698684297865808,
	0.003736240359381998520654927536,
	-0.000916623948045470238763619870,
	0.000199094325044940833965078819,
	-0.000040276384918650072591781859,
	7.76515264697061049477127605790e-6,
	-1.44464794206689070402099225301e-6,
	2.61311930343463958393485241947e-7,
	-4.61833026634844152345304095560e-8,
	8.00253111512943601598732144340e-9,
	-1.36291114862793031395712122089e-9,
	2.28570483090160869607683087722e-10,
	-3.78022521563251805044056974560e-11,
	6.17253683874528285729910462130e-12,
	-9.96019290955316888445830597430e-13,
	1.58953143706980770269506726000e-13,
	-2.51045971047162509999527428316e-14,
	3.92607828989125810013581287560e-15,
	-6.07970619384160374392535453420e-16,
	9.12600607264794717315507477670e-17
};
SChebyshevSeries erfcLess10Cs = {
	erfcLess10Data,
	19,
	-1, 1,
	12
}; 

double Erfc(double x)
{
	const double ax = fabs(x);
	double y;

	if(ax <= 1.0)
	{
	    double t = 2.0*ax - 1.0;
		y = Chebyshev(&erfcLess1Cs, t);
	}
	else if(ax <= 5.0)
	{
	    double ex2 = exp(-x*x);
		double t = 0.5*(ax-3.0);
		y = ex2 * Chebyshev(&erfcLess5Cs, t);
	}
	else if(ax < 10.0)
	{
		double exterm = exp(-x*x) / ax;
	    double t = (2.0*ax - 15.0)/5.0;
		y = exterm * Chebyshev(&erfcLess10Cs, t);
	}
	else
	{
		y = Erfc8(ax);
	}

	if(x < 0.0) return 2-y;
	else return y;
}

double ErfInv(double x)
{
	double y = 0;
	for(unsigned int i = 0; i < 1001; i++)
		y += erfInvSeriesCoeff[i] * pow(x, (double)2*i+1);
	return y;
}

double Erfc8(double x)
{
	return Erfc8Sum(x) * exp(-x*x);
}

double Erfc8Sum(double x)
{
	/* estimates erfc(x) valid for 8 < x < 100 */
	/* This is based on index 5725 in Hart et al */

	static double P[] = {
		2.97886562639399288862,
		7.409740605964741794425,
		6.1602098531096305440906,
		5.019049726784267463450058,
		1.275366644729965952479585264,
		0.5641895835477550741253201704
	};
	static double Q[] = {
		3.3690752069827527677,
		9.608965327192787870698,
		17.08144074746600431571095,
		12.0489519278551290360340491,
		9.396034016235054150430579648,
		2.260528520767326969591866945,
		1.0
	};

	double num=0.0, den=0.0;
	int i;

	num = P[5];
	for (i=4; i>=0; --i) {
		num = x*num + P[i];
	}
	den = Q[6];
	for (i=5; i>=0; --i) {
		den = x*den + Q[i];
	}

	return num/den; 
}
