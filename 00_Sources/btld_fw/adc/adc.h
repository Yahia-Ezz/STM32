#ifndef ADC_H_
#define ADC_H_


#define ADC1_BASE_ADDRESS 0x40012400U

typedef union 
{
    uint32_t R;
    struct 
    {
        uint32_t AWD : 1;       // Analog watchdog flag
        uint32_t EOC : 1;       // End of conversion
        uint32_t JEOC : 1;      // Injected channel end of conversion
        uint32_t JSTRT : 1;     // Injected channel Start flag
        uint32_t STRT : 1;      // Regular channel Start flag
        uint32_t RESERVED : 27;
    }B;
}ADC_SR_t;

typedef union 
{
    uint32_t R;
    struct 
    {
        uint32_t AWDCH : 5;     // Analog watchdog channel select bits 
        uint32_t EOCIE : 1;     // Interrupt enable for EOC
        uint32_t AWDIE : 1;     // Analog watchdog interrupt enable
        uint32_t JEOCIE : 1;    // Interrupt enable for injected channels
        uint32_t SCAN : 1;      // Scan mode 
        uint32_t AWDSGL : 1;    // Enable the watchdog on a single channel in scan mode
        uint32_t JAUTO : 1;     // Automatic Injected Group conversion
        uint32_t DISCEN : 1;    // Discontinuous mode on regular channels
        uint32_t JDISCEN : 1;   // Discontinuous mode on injected channels
        uint32_t DISCNUM : 3;   // Discontinuous mode channel count
        uint32_t DUALMOD : 4;   // Dual mode selection
        uint32_t RESERVED1 : 2; 
        uint32_t JAWDEN : 1;    // Analog watchdog enable on injected channels
        uint32_t AWDEN : 1;     // Analog watchdog enable on regular channels
        uint32_t RESERVED2 : 8;
    }B;
}ADC_CR1_t;

typedef union 
{
    uint32_t R;
    struct
    {
       uint32_t ADON:1;         // A/D converter ON / OFF
       uint32_t CONT:1;         // Continuous conversion
       uint32_t CAL:1;          // A/D Calibration
       uint32_t RSTCAL:1;       // Reset calibration
       uint32_t RESERVED1:4;
       uint32_t DMA:1;          // Direct memory access mode
       uint32_t RESERVED2:2;
       uint32_t ALIGN:1;        // Data alignment
       uint32_t JEXTSEL:3;      // External event select for injected group
       uint32_t JEXTTRIG:1;     // External trigger conversion mode for injected channels
       uint32_t RESERVED3:1;
       uint32_t EXTSEL:3;       // External event select for regular group
       uint32_t EXTTRIG:1;      // External trigger conversion mode for regular channels
       uint32_t JSWSTART:1;     // Start conversion of injected channels
       uint32_t SWSTART:1;      // Start conversion of regular channels
       uint32_t TSVREFE:1;      // Temperature sensor and VREFINT enable
    //    uint32_t RESERVED4:8;
    } B;
}ADC_CR2_t;

typedef union 
{
    uint32_t R;
    struct 
    {
        uint32_t SMP10: 3;      // Channel 10 Sample time selection
        uint32_t SMP11: 3;      // Channel 11 Sample time selection
        uint32_t SMP12: 3;      // Channel 12 Sample time selection
        uint32_t SMP13: 3;      // Channel 13 Sample time selection
        uint32_t SMP14: 3;      // Channel 14 Sample time selection
        uint32_t SMP15: 3;      // Channel 15 Sample time selection
        uint32_t SMP16: 3;      // Channel 16 Sample time selection
        uint32_t SMP17: 3;      // Channel 17 Sample time selection
        uint32_t Reserved: 8;
    }B;
}ADC_SMPR1_t;

typedef union 
{
    uint32_t R;
    struct 
    {
        uint32_t SMP0: 3;      // Channel 0 Sample time selection
        uint32_t SMP1: 3;      // Channel 1 Sample time selection
        uint32_t SMP2: 3;      // Channel 2 Sample time selection
        uint32_t SMP3: 3;      // Channel 3 Sample time selection
        uint32_t SMP4: 3;      // Channel 4 Sample time selection
        uint32_t SMP5: 3;      // Channel 5 Sample time selection
        uint32_t SMP6: 3;      // Channel 6 Sample time selection
        uint32_t SMP7: 3;      // Channel 7 Sample time selection
        uint32_t SMP8: 3;      // Channel 8 Sample time selection
        uint32_t SMP9: 3;      // Channel 9 Sample time selection
        uint32_t Reserved: 2;
    }B;
}ADC_SMPR2_t;

typedef union 
{
    uint32_t R;
    struct 
    {
        uint32_t JOFFSET1:12;       // Data offset for injected channel 1
        uint32_t RESERVED1:20;
        uint32_t JOFFSET2:12;       // Data offset for injected channel 2
        uint32_t RESERVED2:20;
        uint32_t JOFFSET3:12;       // Data offset for injected channel 3
        uint32_t RESERVED3:20;
        uint32_t JOFFSET4:12;       // Data offset for injected channel 4
        uint32_t RESERVED4:20;
    }B;
}ADC_JOFRx_t;


typedef union 
{
    uint32_t R;
    struct 
    {
        uint32_t HT:12;         // Analog watchdog high threshold
        uint32_t RESERVED:20;
    }B;
}ADC_HTR_t;
typedef union 
{
    uint32_t R;
    struct 
    {
        uint32_t LT:12;         // Analog watchdog low threshold 
        uint32_t RESERVED:20;
    }B;
}ADC_LTR_t;

typedef union 
{
    uint32_t R;
    struct 
    {
        uint32_t SQ13: 5;       // 13th conversion in regular sequence
        uint32_t SQ14: 5;       // 14th conversion in regular sequence
        uint32_t SQ15: 5;       // 15th conversion in regular sequence
        uint32_t SQ16: 5;       // 16th conversion in regular sequence
        uint32_t L: 4;          //Regular channel sequence length
        uint32_t Reserved: 8;
    }B;
}ADC_SQR1_t;

typedef union 
{
    uint32_t R;
    struct 
    {
        uint32_t SQ7: 5;           // 7th conversion in regular sequence    
        uint32_t SQ8: 5;           // 8th conversion in regular sequence
        uint32_t SQ9: 5;           // 9th conversion in regular sequence
        uint32_t SQ10: 5;          // 10th conversion in regular sequence
        uint32_t SQ11: 5;          // 11th conversion in regular sequence
        uint32_t SQ12: 5;          // 12th conversion in regular sequence
        uint32_t Reserved: 2;
    }B;
}ADC_SQR2_t;
typedef union 
{
    uint32_t R;
    struct 
    {
        uint32_t SQ1: 5;           // 1st conversion in regular sequence
        uint32_t SQ2: 5;           // 2nd conversion in regular sequence
        uint32_t SQ3: 5;           // 3rd conversion in regular sequence
        uint32_t SQ4: 5;           // 4th conversion in regular sequence
        uint32_t SQ5: 5;           // 5th conversion in regular sequence
        uint32_t SQ6: 5;           // 6th conversion in regular sequence
        uint32_t Reserved: 2;
    }B;
}ADC_SQR3_t;

typedef union 
{
    uint32_t R;
    struct 
    {
        uint32_t JSQ1: 5;       // First conversion in injected sequence
        uint32_t JSQ2: 5;       // Second conversion in injected sequence
        uint32_t JSQ3: 5;       // Third conversion in injected sequence
        uint32_t JSQ4: 5;       // Fourth conversion in injected sequence
        uint32_t JL: 2;         // Injected sequence length
        uint32_t Reserved: 10;
    }B;
}ADC_JSQR_t;

typedef union 
{
    uint32_t R;
    struct 
    {
        uint32_t JDATA1: 16;            //Injected data 1 
        uint32_t RESERVED1: 16;
        uint32_t JDATA2: 16;            //Injected data 2
        uint32_t RESERVED2: 16;
        uint32_t JDATA3: 16;            //Injected data 3
        uint32_t RESERVED3: 16;
        uint32_t JDATA4: 16;            //Injected data 4
        uint32_t RESERVED4: 16;
    }B;
}ADC_JDRx_t;

typedef union 
{
    uint32_t R;
    struct 
    {
        uint32_t DATA: 16;      // Regular data
        uint32_t ADC2DATA: 16;   // ADC2 data
    }B;
}ADC_DR_t;

typedef struct 
{
    ADC_SR_t    SR;
    ADC_CR1_t   CR1;
    ADC_CR2_t   CR2;
    ADC_SMPR1_t SMPR1;
    ADC_SMPR2_t SMPR2;
    ADC_JOFRx_t JOFRx;
    ADC_HTR_t   HTR;
    ADC_LTR_t   LTR;
    ADC_SQR1_t  SQR1;
    ADC_SQR2_t  SQR2;
    ADC_SQR3_t  SQR3;
    ADC_JSQR_t  JSQR;
    ADC_JDRx_t  JDRx;
    ADC_DR_t    DR;
}ADC_PERIPHERAL_t;

void ADC_init(void);
void ADC_InterruptHandler(void);

#endif