#include "Core/Services/Matrix1000FactoryPatchNames.h"
#include "Core/Models/PatchModel.h"
#include "Shared/Definitions/Matrix1000Limits.h"

namespace Core
{
    namespace
    {
        constexpr int kRomBankCount = 8;
        constexpr int kPatchesPerBank = 100;
        constexpr int kFirstRomBank = 2;

        // Layout: bank-major, index = (bank - 2) * 100 + patch
        constexpr const char* kRomNames[kRomBankCount * kPatchesPerBank] = {
            //---------------------------- Bank 2
            "OBXA-11",       // B2P00
            "OBXA-12",       // B2P01
            "OBXA-A2",       // B2P02
            "OBXA-A7",       // B2P03
            "OBXA-B7",       // B2P04
            "OBXA-B8",       // B2P05
            "OBXA-C2",       // B2P06
            "OBXA-C4",       // B2P07
            "OBXA-C6",       // B2P08
            "OBXA-C7",       // B2P09
            "OBXA-C8",       // B2P10
            "OBXA-D2",       // B2P11
            "OBXA-D3",       // B2P12
            "OBXA-D4",       // B2P13
            "OBXA-D5",       // B2P14
            "OBXA-D6",       // B2P15
            "OBXA-D7",       // B2P16
            "OBXA-D8",       // B2P17
            "OBXA-9\"",      // B2P18
            "OBCA-RE",       // B2P19
            "OBXJMP",        // B2P20
            "*'ANGEL",       // B2P21
            "+ ZETA +",      // B2P22
            "1984SWP6",      // B2P23
            "WAVES",         // B2P24
            "80MS DDL",      // B2P25
            "SYNTH",         // B2P26
            "AERIAL",        // B2P27
            "ALIENSWP",      // B2P28
            "AMBIANCE",      // B2P29
            "ANAFTST*",      // B2P30
            "ANAHARP",       // B2P31
            "ANALOG B",      // B2P32
            "ANAXYLO*",      // B2P33
            "ANGELS",        // B2P34
            "APOLLO",        // B2P35
            "ARCANGEL",      // B2P36
            "ARGEX-1",       // B2P37
            "ARGON7",        // B2P38
            "ATYPICAL",      // B2P39
            "AW WHY ?",      // B2P40
            "BENSHIMR",      // B2P41
            "BEOWCOMP",      // B2P42
            "BILLY",         // B2P43
            "BLASZZ",        // B2P44
            "BLOCKOUT",      // B2P45
            "BOEPTYN*",      // B2P46
            "BOTTLES",       // B2P47
            "BOUNCE*",       // B2P48
            "BRASSVOX",      // B2P49
            "BRILLANT",      // B2P50
            "BROADWAY",      // B2P51
            "BS ETAK*",      // B2P52
            "BURNHOUS",      // B2P53
            "CAMERA 1",      // B2P54
            "CHIME 1",       // B2P55
            "CHIME 2",       // B2P56
            "CHUNK",         // B2P57
            "CMI HIGH",      // B2P58
            "COEAUR 1",      // B2P59
            "COLONY 9",      // B2P60
            "CRYSLAKE",      // B2P61
            "CS-80",         // B2P62
            "DEACON",        // B2P63
            "DEJAVUE'",      // B2P64
            "DIDIER",        // B2P65
            "DISTANCE",      // B2P66
            "DMACHINE",      // B2P67
            "DREAMER",       // B2P68
            "DREEMER",       // B2P69
            "DUCKTIME",      // B2P70
            "DUNK IT",       // B2P71
            "ENO1",          // B2P72
            "ECHOSYN",       // B2P73
            "ECHOTRON",      // B2P74
            "EGYPT",         // B2P75
            "EP SWEP*",      // B2P76
            "EPCH+BRZ",      // B2P77
            "EPDSTRT*",      // B2P78
            "ESQ-1",         // B2P79
            "ETHEREE",       // B2P80
            "FAKE DDL",      // B2P81
            "FIFTHS",        // B2P82
            "FLOATONG",      // B2P83
            "FLPFLOP*",      // B2P84
            "FLY TO",        // B2P85
            "FM BASS",       // B2P86
            "FUNDO",         // B2P87
            "*FUNK ART",     // B2P88
            "FUNKAY",        // B2P89
            "FURYO",         // B2P90
            "FWEEP",         // B2P91
            "S.1",           // B2P92
            "GALACTIC",      // B2P93
            "GALLOP *",      // B2P94
            "GENIVEEV",      // B2P95
            "GENVIV",        // B2P96
            "GENVIV*",       // B2P97
            "GIRLSWEP",      // B2P98
            "GOOD BED",      // B2P99

            //---------------------------- Bank 3
            "GOODTIME",      // B3P00
            "GROTTO",        // B3P01
            "HACKETT",       // B3P02
            "HALO",          // B3P03
            "HARMOVOX",      // B3P04
            "HARPOON",       // B3P05
            "HELI-IN",       // B3P06
            "HOMETOWN",      // B3P07
            "INTERSTL",      // B3P08
            "ITSONICE",      // B3P09
            "JAZZQUIT",      // B3P10
            "JM JARRE",      // B3P11
            "JOHN B'S",      // B3P12
            "KCEPMAX*",      // B3P13
            "KCEPSAW*",      // B3P14
            "KCHSYNC*",      // B3P15
            "KIRKLAND",      // B3P16
            "LDSUBHRM",      // B3P17
            "LIKETHIS",      // B3P18
            "LSTLAUGH",      // B3P19
            "LUN'AIR",       // B3P20
            "M-CHOIR",       // B3P21
            "MAGICAL",       // B3P22
            "MARIN",         // B3P23
            "MATMODUL",      // B3P24
            "MATRIX 1",      // B3P25
            "MATRIX 2",      // B3P26
            "METABOAD",      // B3P27
            "METABRD",       // B3P28
            "MINDSEAR",      // B3P29
            "MONSTER",       // B3P30
            "MR KYRIE",      // B3P31
            "MUSICBOX",      // B3P32
            "NAUTILUS",      // B3P33
            "NEW VOX",       // B3P34
            "NEWSOUND",      // B3P35
            "NIGHTPAD",      // B3P36
            "OB SWEEP",      // B3P37
            "OB VOX*",       // B3P38
            "OB-INTRO",      // B3P39
            "OBXA-B2",       // B3P40
            "OBXA-B4",       // B3P41
            "OCTAVIA",       // B3P42
            "OPEN AIR",      // B3P43
            "ORDINARY",      // B3P44
            "P CHORD",       // B3P45
            "P-PLUCK",       // B3P46
            "PAD",           // B3P47
            "PERC S",        // B3P48
            "PHASE 5",       // B3P49
            "PHASECHO",      // B3P50
            "PICKY",         // B3P51
            "PIPESTR",       // B3P52
            "PN/FMSWP",      // B3P53
            "POLCHOIR",      // B3P54
            "POWER",         // B3P55
            "PROPHET1",      // B3P56
            "PROPHET5",      // B3P57
            "PROPHETV",      // B3P58
            "PSYLITIS",      // B3P59
            "REZ*PULS",      // B3P60
            "REZTFUL",       // B3P61
            "SAMPLE",        // B3P62
            "SATURN",        // B3P63
            "SCIENCE",       // B3P64
            "SCRITTI+",      // B3P65
            "SECRETS",       // B3P66
            "SENSIT 2",      // B3P67
            "SENSITIV",      // B3P68
            "SEQUINOX",      // B3P69
            "SHANKAR",       // B3P70
            "SHIMMER",       // B3P71
            "SHIMRING",      // B3P72
            "SHIVERS",       // B3P73
            "SKRCHTN*",      // B3P74
            "SKTSOKY*",      // B3P75
            "SKY HIGH",      // B3P76
            "SKYVOICE",      // B3P77
            "SLAPBACH",      // B3P78
            "SLIDSTG",       // B3P79
            "SLOW CRY",      // B3P80
            "SLOWATER",      // B3P81
            "SMTHSQ2*",      // B3P82
            "SOFT MIX",      // B3P83
            "SOUNDPAD",      // B3P84
            "SOUNDTR[",      // B3P85
            "SPACE",         // B3P86
            "SPACE/CO",      // B3P87
            "SPARKLES",      // B3P88
            "SPLASH 1",      // B3P89
            "STELLAR*",      // B3P90
            "STUGROWL",      // B3P91
            "STYX",          // B3P92
            "SUBMARIN",      // B3P93
            "SUNDAY",        // B3P94
            "SUSSUDIO",      // B3P95
            "SWEPCORD",      // B3P96
            "SWRLEKO*",      // B3P97
            "SYN BOX*",      // B3P98
            "SYNCAGE*",      // B3P99

            //---------------------------- Bank 4
            "TOTOHORN",      // B4P00
            "OBXA-13",       // B4P01
            "OBXA-A1",       // B4P02
            "OBXA-B1",       // B4P03
            "OBXA-C1",       // B4P04
            "OBXA-D1",       // B4P05
            "6R BRASS",      // B4P06
            "AGRESORN",      // B4P07
            "ALASKA",        // B4P08
            "ANA HIT*",      // B4P09
            "ANASUTL*",      // B4P10
            "B'ARI/S2",      // B4P11
            "B/D-ANA*",      // B4P12
            "BAGPIPES",      // B4P13
            "BARISAX",       // B4P14
            "BASCLRNT",      // B4P15
            "BASSCLAR",      // B4P16
            "BASSOON",       // B4P17
            "BENDHORN",      // B4P18
            "BIGBRA$$",      // B4P19
            "BONES",         // B4P20
            "BRASRAMP",      // B4P21
            "BTRASSVOX",     // B4P22
            "BRASSY",        // B4P23
            "BRAZEN",        // B4P24
            "BRECHER",       // B4P25
            "BRTH FLT",      // B4P26
            "BRUTUS",        // B4P27
            "BRZIVIV*",      // B4P28
            "BUCHANN*",      // B4P29
            "BUZREED",       // B4P30
            "CHROMA-S",      // B4P31
            "CLARINET",      // B4P32
            "CRAZHORN",      // B4P33
            "CS-80",         // B4P34
            "CUIVRE((",      // B4P35
            "DBLREED*",      // B4P36
            "EASTREED",      // B4P37
            "EDGY",          // B4P38
            "ENSEMBL*",      // B4P39
            "EUROPE",        // B4P40
            "EWF HORN",      // B4P41
            "EZYBRASS",      // B4P42
            "FACTORY",       // B4P43
            "FIFTHS",        // B4P44
            "FLGLHORN",      // B4P45
            "FLOOT",         // B4P46
            "FLUGELHN",      // B4P47
            "FLUGLE",        // B4P48
            "FLUTE",         // B4P49
            "FLUTE TR",      // B4P50
            "FLUTE.",        // B4P51
            "FLUTES",        // B4P52
            "FLUTEY",        // B4P53
            "FLUX",          // B4P54
            "FM BRAZ",       // B4P55
            "FM DELAY",      // B4P56
            "FR.HORN",       // B4P57
            "FNRCHRN*",      // B4P58
            "FTHWEEL*",      // B4P59
            "FUE.JAPN",      // B4P60
            "FUSION",        // B4P61
            "FWEEP",         // B4P62
            "GABRIEL",       // B4P63
            "GO BED",        // B4P64
            "GOLIATH",       // B4P65
            "HORN'EM",       // B4P66
            "HORN-1",        // B4P67
            "HORNENS",       // B4P68
            "HORNFALL",      // B4P69
            "HORNSAS",       // B4P70
            "HORNY",         // B4P71
            "HRNSHAKE",      // B4P72
            "J HAMMER",      // B4P73
            "JTULLFLT",      // B4P74
            "JUBILEE",       // B4P75
            "KLARYNET",      // B4P76
            "KORGHORN",      // B4P77
            "LYRICON",       // B4P78
            "MATRONE",       // B4P79
            "MELFAZE*",      // B4P80
            "MELOHORN",      // B4P81
            "METHENY5",      // B4P82
            "MUTETRPT",      // B4P83
            "OB BRASS",      // B4P84
            "OB-8",          // B4P85
            "OBERHORN",      // B4P86
            "OBOE",          // B4P87
            "OCT.BRS",       // B4P88
            "OCTAFLUT",      // B4P89
            "OCTAHORN",      // B4P90
            "ORIENT",        // B4P91
            "PEDSWP*",       // B4P92
            "PEG-BRS",       // B4P93
            "PYRMFLT*",      // B4P94
            "RAHOOOL*",      // B4P95
            "RECORDER",      // B4P96
            "RELVELHO",      // B4P97
            "RICHCORD",      // B4P98
            "ROMAN",         // B4P99

            //---------------------------- Bank 5
            "SEXAFOAM",      // B5P00
            "SLO HRN",       // B5P01
            "SOPIPES",       // B5P02
            "SPATBRS*",      // B5P03
            "SQUARDOU",      // B5P04
            "STAB",          // B5P05
            "STAB-BRS",      // B5P06
            "STEPS 2.",      // B5P07
            "STUFLUTE",      // B5P08
            "SWRLYBRD",      // B5P09
            "SYN SAX*",      // B5P10
            "SYNBASS",       // B5P11
            "SYNBONE",       // B5P12
            "SYNBRSS*",      // B5P13
            "SYNHORN",       // B5P14
            "TBRAZZ",        // B5P15
            "TENOR",         // B5P16
            "TOTOAL",        // B5P17
            "TOUCH+GO",      // B5P18
            "TRILLFLT",      // B5P19
            "TRMBONE*",      // B5P20
            "TROMBONE",      // B5P21
            "TRUMPETS",      // B5P22
            "TRUPT-EU",      // B5P23
            "* 99 *",        // B5P24
            "TUBA 2",        // B5P25
            "OBXA-A8",       // B5P26
            "ELEAD*",        // B5P27
            "BDTH-2",        // B5P28
            "BIRDY",         // B5P29
            "BRECKERL",      // B5P30
            "CASTILLO",      // B5P31
            "CHICK",         // B5P32
            "DESTROY+",      // B5P33
            "DIGRUNGE",      // B5P34
            "DRAGON-3",      // B5P35
            "DXINDIAN",      // B5P36
            "FEEDBAK6",      // B5P37
            "FEEDBAK8",      // B5P38
            "FEEDGIT",       // B5P39
            "FIFTH I%",      // B5P40
            "FIFTHLIX",      // B5P41
            "GLASLEED",      // B5P42
            "GROWLBRS",      // B5P43
            "H-LEAD",        // B5P44
            "HILEED 6",      // B5P45
            "J HAMMER",      // B5P46
            "JAKOLEED",      // B5P47
            "JAN LEAD",      // B5P48
            "JAZZ",          // B5P49
            "JIMY'SRG",      // B5P50
            "KC LEAD*",      // B5P51
            "KIDDING?",      // B5P52
            "LEAD+PRT",      // B5P53
            "LEAD-1",        // B5P54
            "LEAD-3",        // B5P55
            "LEED-1",        // B5P56
            "LEED-2",        // B5P57
            "LYLE 2",        // B5P58
            "LYLE 3 M",      // B5P59
            "METHENEY",      // B5P60
            "METLSOLO",      // B5P61
            "MILESCOM",      // B5P62
            "MINIMOGG",      // B5P63
            "MINIMOOG",      // B5P64
            "MONOSTRG",      // B5P65
            "NASTY",         // B5P66
            "OB LEAD*",      // B5P67
            "OSC SYNC",      // B5P68
            "PANFLOET",      // B5P69
            "PINKLEAD",      // B5P70
            "POWRSOLO",      // B5P71
            "PRSSLIDE",      // B5P72
            "QUINCY",        // B5P73
            "RECORDER",      // B5P74
            "REZLEAD*",      // B5P75
            "SAWLEAD*",      // B5P76
            "SITAR",         // B5P77
            "SMOOTH",        // B5P78
            "SMUTHSQ*",      // B5P79
            "SOLO",          // B5P80
            "SOLODARM",      // B5P81
            "SOLOPROF",      // B5P82
            "SOLOSYNC",      // B5P83
            "SOLOW*",        // B5P84
            "SOPIPES",       // B5P85
            "SPITLEED",      // B5P86
            "SQARELED",      // B5P87
            "STUVIB",        // B5P88
            "SUSGUIT",       // B5P89
            "SNTHE 5",       // B5P90
            "UKSOLO",        // B5P91
            "UNIBASS",       // B5P92
            "UNIWAVE",       // B5P93
            "WAKEMANS",      // B5P94
            "WEIRDPRC",      // B5P95
            "WHISTLER",      // B5P96
            "WINAND 1",      // B5P97
            "XA'SOLO",       // B5P98
            "ZAW'QART",      // B5P99

            //---------------------------- Bank 6
            "OBXA-10",       // B6P00
            "OBXA-14",       // B6P01
            "OBXA-A3",       // B6P02
            "OBXA-B3",       // B6P03
            "OBXA-B6",       // B6P04
            "OBXA-C3",       // B6P05
            "OBXA",          // B6P06
            "OBXA-6",        // B6P07
            "(ARCO)01",      // B6P08
            "*'CANOPY",      // B6P09
            "1000STRG",      // B6P10
            "TOP",           // B6P11
            "2000STRG",      // B6P12
            "AGITATO*",      // B6P13
            "ALL LOVE",      // B6P14
            "ALT84TOP",      // B6P15
            "BED TRAK",      // B6P16
            "BLACSEAM",      // B6P17
            "BOW IT",        // B6P18
            "BOW VIOL",      // B6P19
            "CELLO",         // B6P20
            "CHAMBER",       // B6P21
            "CHILLO",        // B6P22
            "CLASSIKA",      // B6P23
            "CONCERT",       // B6P24
            "DEEPCAVE",      // B6P25
            "DEPTHS",        // B6P26
            "DLAYSTR*",      // B6P27
            "DONSTRIG",      // B6P28
            "DOU'CIEL",      // B6P29
            "DUNGEON",       // B6P30
            "DYNASTY",       // B6P31
            "E.VIOLIN",      // B6P32
            "FAMUS*OB",      // B6P33
            "FORESTS",       // B6P34
            "GRANULES",      // B6P35
            "GREAT\"OB",     // B6P36
            "HARMONIC",      // B6P37
            "ICY-CHRD",      // B6P38
            "INDNSTRG",      // B6P39
            "LOWSTRNG",      // B6P40
            "LOYAL",         // B6P41
            "LUSHNESS",      // B6P42
            "LYLE-8VA",      // B6P43
            "MELLO=14",      // B6P44
            "MKSINGS",       // B6P45
            "MONEY $$",      // B6P46
            "MUTEDSTR",      // B6P47
            "MZSTRING",      // B6P48
            "NOBLE",         // B6P49
            "NOISTGS",       // B6P50
            "OB A3PD*",      // B6P51
            "OB-STR1N",      // B6P52
            "OB-STRGS",      // B6P53
            "OBSTRING",      // B6P54
            "OBXA-A6",       // B6P55
            "OCARINA",       // B6P56
            "OCHESTRY",      // B6P57
            "OPENSTRG",      // B6P58
            "ORCH*",         // B6P59
            "ORIENT",        // B6P60
            "PITZ STR",      // B6P61
            "PIZZ^+P2",      // B6P62
            "PLANET P",      // B6P63
            "POLSTRG2",      // B6P64
            "PROHET-5",      // B6P65
            "PROPHET5",      // B6P66
            "RID ZEP",       // B6P67
            "ROYAL PH",      // B6P68
            "RP STRG5",      // B6P69
            "SECRETS'",      // B6P70
            "SHARPBOW",      // B6P71
            "SHIFT",         // B6P72
            "SHRTSTRG",      // B6P73
            "SINGS",         // B6P74
            "SLIDSTG",       // B6P75
            "SLOW BOW",      // B6P76
            "SLOW CRY",      // B6P77
            "SMASH",         // B6P78
            "SOLEMN",        // B6P79
            "SOLEMNIS",      // B6P80
            "SOLO",          // B6P81
            "SOUNDTR",       // B6P82
            "SOUNDTRK",      // B6P83
            "SOUNDTR[",      // B6P84
            "SRTRONGS",      // B6P85
            "STAND UP",      // B6P86
            "STR END*",      // B6P87
            "STR-8VA",       // B6P88
            "STRANGER",      // B6P89
            "STREENG",       // B6P90
            "STREENGS",      // B6P91
            "STRING 2",      // B6P92
            "STRING 6",      // B6P93
            "STRING 7",      // B6P94
            "STRING 8",      // B6P95
            "STRING S",      // B6P96
            "STRING\"8",     // B6P97
            "STRING-1",      // B6P98
            "STRINGER",      // B6P99

            //---------------------------- Bank 7
            "**A!A!**",      // B7P00
            "2600-2",        // B7P01
            "AGREBASS",      // B7P02
            "ANTEATER",      // B7P03
            "ARP-2600",      // B7P04
            "ATYPBASS",      // B7P05
            "AXXE",          // B7P06
            "BARISAX",       // B7P07
            "BASS PAD",      // B7P08
            "BASS SYN",      // B7P09
            "BASS ZZT",      // B7P10
            "BASS-11",       // B7P11
            "BASSA",         // B7P12
            "BASSCLAR",      // B7P13
            "TUBULAR",       // B7P14
            "BASSE OA",      // B7P15
            "BASSGTAR",      // B7P16
            "BASSHIPO",      // B7P17
            "BASSHORN",      // B7P18
            "BASSVIOL",      // B7P19
            "BASSVOX",       // B7P20
            "BIG PIK",       // B7P21
            "BIRDLAND",      // B7P22
            "BOLUBASS",      // B7P23
            "BOTBASS",       // B7P24
            "BOWBASS",       // B7P25
            "BRAAS",         // B7P26
            "BS/STRG*",      // B7P27
            "UNI BASS",      // B7P28
            "CLAVBASS",      // B7P29
            "BOUBLEBS",      // B7P30
            "DUCK 2",        // B7P31
            "DUCKBASS",      // B7P32
            "EARTHESS",      // B7P33
            "ELC BASS",      // B7P34
            "ELEC BS*",      // B7P35
            "FANKNBAZ",      // B7P36
            "FAZ BASS",      // B7P37
            "FLOORIT",       // B7P38
            "FRET EKO",      // B7P39
            "FRET NOT",      // B7P40
            "FUNK BAZ",      // B7P41
            "HARMBAS5",      // B7P42
            "HISBASS",       // B7P43
            "HOTBODOM",      // B7P44
            "JAN BASS",      // B7P45
            "JOCKO",         // B7P46
            "JOCKO 2",       // B7P47
            "LEEDBASS",      // B7P48
            "LUMPBASS",      // B7P49
            "MINIBASS",      // B7P50
            "MONO BS*",      // B7P51
            "MOOGER",        // B7P52
            "MOOOG_B",       // B7P53
            "MUFFEL",        // B7P54
            "NOISBASS",      // B7P55
            "OCTABASS",      // B7P56
            "ORBASS",        // B7P57
            "OW BASS",       // B7P58
            "PABASS*",       // B7P59
            "PLUCK-BS",      // B7P60
            "POLBASS1",      // B7P61
            "POLYBASS",      // B7P62
            "PUKBASS",       // B7P63
            "R+B",           // B7P64
            "RAGABASS",      // B7P65
            "VELBASS",       // B7P66
            "RUBBER",        // B7P67
            "SEQUBASS",      // B7P68
            "VELGROWL",      // B7P69
            "SINCBASS",      // B7P70
            "WAPBASS",       // B7P71
            "SLAP 1",        // B7P72
            "SLAP 2",        // B7P73
            "SLIDER",        // B7P74
            "SLOWBASS",      // B7P75
            "SNTHBS1*",      // B7P76
            "SOFTBASS",      // B7P77
            "SPITBASS",      // B7P78
            "SQUISBAZ",      // B7P79
            "STAND UP",      // B7P80
            "STBASS",        // B7P81
            "STR.BASS",      // B7P82
            "STRANGTK",      // B7P83
            "STRIBASS",      // B7P84
            "WET BAZ",       // B7P85
            "STRINGBZ",      // B7P86
            "SUGITA\"",      // B7P87
            "SUPPORT",       // B7P88
            "SWELLBAZ",      // B7P89
            "SWP.BASS",      // B7P90
            "SYBASS 2",      // B7P91
            "SYN BS2*",      // B7P92
            "SYN BS3*",      // B7P93
            "SYN BS4*",      // B7P94
            "SYNCBASS",      // B7P95
            "TAURUS",        // B7P96
            "TENU'OB2",      // B7P97
            "WIPBASS",       // B7P98
            "TIKBASS",       // B7P99

            //---------------------------- Bank 8
            "AK-48",         // B8P00
            "APORT",         // B8P01
            "BALLGAME",      // B8P02
            "BANJO",         // B8P03
            "BASSDRUM",      // B8P04
            "BDTH-1",        // B8P05
            "BELL 1",        // B8P06
            "BELLIKE",       // B8P07
            "BELLS",         // B8P08
            "BELLS-GS",      // B8P09
            "BI-PLANE",      // B8P10
            "BOTTLES",       // B8P11
            "BTMEHRDR",      // B8P12
            "BURST 1",       // B8P13
            "CASCAD'4",      // B8P14
            "CHIMES",        // B8P15
            "CHIMES*",       // B8P16
            "CHOPPERZ",      // B8P17
            "COINOP 3",      // B8P18
            "COPOLIPS",      // B8P19
            "CRAZYMAN",      // B8P20
            "CRICKET",       // B8P21
            "CROZTALK",      // B8P22
            "DB BELL",       // B8P23
            "DIDIER",        // B8P24
            "DREAMING",      // B8P25
            "DRIFTER*",      // B8P26
            "DRUMPOP",       // B8P27
            "DUNDERZ",       // B8P28
            "DX-PLUCK",      // B8P29
            "TURBO",         // B8P30
            "FALLCHYM",      // B8P31
            "FIREBALL",      // B8P32
            "FLAME ON",      // B8P33
            "FLEXTONE",      // B8P34
            "FMPLUKS",       // B8P35
            "FURYO 2",       // B8P36
            "G.S.2",         // B8P37
            "G.S.3",         // B8P38
            "GLOCK",         // B8P39
            "WINDS",         // B8P40
            "HAUNTING",      // B8P41
            "HEART",         // B8P42
            "ZAP",           // B8P43
            "HORRORS",       // B8P44
            "HOWITZER",      // B8P45
            "HVN+HELL",      // B8P46
            "INDIAN",        // B8P47
            "TOP-GUN*",      // B8P48
            "INSIDES",       // B8P49
            "JETTZ 3",       // B8P50
            "JUNKANOO",      // B8P51
            "WETFEET",       // B8P52
            "KINGONG",       // B8P53
            "KONTAKTE",      // B8P54
            "LCTRCUTE",      // B8P55
            "WHIZZ",         // B8P56
            "LFO ART",       // B8P57
            "LFOMALET",      // B8P58
            "LIFTOFF",       // B8P59
            "LOOPBELL",      // B8P60
            "LYLE 3 P",      // B8P61
            "LYLES'",        // B8P62
            "THUNDRUS",      // B8P63
            "MACHINSM",      // B8P64
            "MANIAC*",       // B8P65
            "MARIMA",        // B8P66
            "MOFO",          // B8P67
            "MEMORIES",      // B8P68
            "MOUNTAIN",      // B8P69
            "MRIMBAH*",      // B8P70
            "NASTEEZ",       // B8P71
            "WARNINGS",      // B8P72
            "NOISE-DN",      // B8P73
            "NOISSWEP*",     // B8P74
            "NOIZGATE",      // B8P75
            "NTHENEWS",      // B8P76
            "NUKE EM'",      // B8P77
            "OCIEAN",        // B8P78
            "OCEANWAV",      // B8P79
            "OOZES 3",       // B8P80
            "PHASES*",       // B8P81
            "PINWHEEL",      // B8P82
            "PLUCK",         // B8P83
            "POLBELS2",      // B8P84
            "POLNOISE",      // B8P85
            "PORTAL",        // B8P86
            "PSYCHYM",       // B8P87
            "RAINECHO",      // B8P88
            ")RAPIST(",      // B8P89
            "WATER",         // B8P90
            "RUBRTOMS",      // B8P91
            "SATURDAY",      // B8P92
            "SCRATCH",       // B8P93
            "SEQEUNCE",      // B8P94
            "SGUSTING",      // B8P95
            "SHRNKRAY",      // B8P96
            "SIMONISK",      // B8P97
            "SMASH*",        // B8P98
            "SMPLTHIS",      // B8P99

            //---------------------------- Bank 9
            "PAPANO 4",      // B9P00
            "MIKPIANO",      // B9P01
            "HONOCLAB",      // B9P02
            "MR.ROGRS",      // B9P03
            "MTL PNO*",      // B9P04
            "MUSETTE",       // B9P05
            "MUTDCLV*",      // B9P06
            "MUTRONO",       // B9P07
            "NYLNPIK*",      // B9P08
            "NYLNPK2*",      // B9P09
            "NYLON 12",      // B9P10
            "OB8 JUMP",      // B9P11
            "OBNOXVOX",      // B9P12
            "OBXA-B5",       // B9P13
            "ODX 7",         // B9P14
            "OORGAN",        // B9P15
            "LAZ HARP",      // B9P16
            "ORGAN 9",       // B9P17
            "ORGAN-1",       // B9P18
            "ORGAN-1P",      // B9P19
            "ORGAN-2",       // B9P20
            "ORGANISM",      // B9P21
            "AKOUSTIK",      // B9P22
            "ORGNIZE*",      // B9P23
            "P.ORGAN",       // B9P24
            "P.ORGAN4",      // B9P25
            "P.ORGAN5",      // B9P26
            "PA ANO 5",      // B9P27
            "HARPO",         // B9P28
            "PAPANO 7",      // B9P29
            "LULLABOX",      // B9P30
            "PERCCLAV",      // B9P31
            "PERCPNO",       // B9P32
            "PIANITAR",      // B9P33
            "PIANO",         // B9P34
            "PIANO BO",      // B9P35
            "PIANOLA",       // B9P36
            "B-3.1",         // B9P37
            "PINPIANO",      // B9P38
            "PIPEORG.",      // B9P39
            "PIPEORG:",      // B9P40
            "PIPES",         // B9P41
            "PIPSTRNG",      // B9P42
            "PIRATES!",      // B9P43
            "PNO-ELEC",      // B9P44
            "POLPIANP",      // B9P45
            "PRELUDE1",      // B9P46
            "PRESLEZ1",      // B9P47
            "PROFIT",        // B9P48
            "PROPH W",       // B9P49
            "PROPHET",       // B9P50
            "RESPIANO",      // B9P51
            "RMIPIANO",      // B9P52
            "ROADS",         // B9P53
            "SAL00N 5",      // B9P54
            "SALOON 3",      // B9P55
            "B-3.2",         // B9P56
            "SALOON 7",      // B9P57
            "SAMPLORG",      // B9P58
            "SAMSGRND",      // B9P59
            "SITAR I",       // B9P60
            "SMTHSQ2*",      // B9P61
            "SPANIEL",       // B9P62
            "SPRPRTS*",      // B9P63
            "B-3.3",         // B9P64
            "B3+LSLIE",      // B9P65
            "STRGTR2*",      // B9P66
            "SYN CLAV",      // B9P67
            "BELLS",         // B9P68
            "SYNLUTH",       // B9P69
            "BLABINET",      // B9P70
            "SYNPIANO",      // B9P71
            "CELESTE",       // B9P72
            "CHIMES",        // B9P73
            "TINEOUT",       // B9P74
            "TONYPIAN",      // B9P75
            "TOYPIANO",      // B9P76
            "TWINSTRG",      // B9P77
            "CHURCH",        // B9P78
            "VIBECHOES",     // B9P79
            "VIBES",         // B9P80
            "CLAV B6",       // B9P81
            "CLAVI 2",       // B9P82
            "WA CLAB*",      // B9P83
            "CLAVINET",      // B9P84
            "WHAANO",        // B9P85
            "WHY FM",        // B9P86
            "CLICKORG",      // B9P87
            "WURLI8",        // B9P88
            "CLUBS",         // B9P89
            "WURLY 2",       // B9P90
            "WURLY 3",       // B9P91
            "X-GRAND",       // B9P92
            "XA'ORGAN",      // B9P93
            "YOUREYES",      // B9P94
            "ZITHER",        // B9P95
            "CORDINE1",      // B9P96
            "D'AMMOND",      // B9P97
            "GREEZY1",       // B9P98
            "GRNDR 6*"       // B9P99
        };
    } // namespace

    juce::String Matrix1000FactoryPatchNames::nameFor(int bank, int patch) noexcept
    {
        if (bank < kFirstRomBank || bank > Matrix1000Limits::kMaxBankNumber)
            return {};

        if (patch < Matrix1000Limits::kMinPatchNumber || patch > Matrix1000Limits::kMaxPatchNumber)
            return {};

        const int index = (bank - kFirstRomBank) * kPatchesPerBank + patch;
        auto name = juce::String(kRomNames[index]);
        if (name.length() > PatchModel::kNameLength)
            name = name.substring(0, PatchModel::kNameLength);
        return name;
    }
}

