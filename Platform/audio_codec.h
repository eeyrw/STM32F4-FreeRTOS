/**
  ******************************************************************************
  * @file    stm324xg_usb_audio_codec.h
  * @author  MCD Application Team
  * @version v1.2.1
  * @date    06-November-2015
  * @brief   This file contains all the functions prototypes for the audio codec
  *          low layer driver.
  *            
  *     *******************************************************************  
  *                           IMPORTANT NOTES
  *     *******************************************************************  
  *     This file is extracted and modified from the audio codec driver
  *     "stm324xg_eval_audio_codec.c" provided by STMicroelectronics for
  *     the STM32F4xx family (in folder Utilities\STM32_EVAL\STM3240_41_G_EVAL).
  *     This modified driver is intended for use ONLY in the scope of the
  *     USB Device Audio Example.
  *        
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2015 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                      <http://www.st.com/SLA0044>
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AUDIOCODEC_H
#define __AUDIOCODEC_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/*------------------------------------
             CONFIGURATION: Audio Codec Driver Configuration parameters
                                      ----------------------------------------*/
//#define AUDIO_USE_MACROS

/* Audio Transfer mode (DMA, Interrupt or Polling) */
#define AUDIO_MAL_MODE_NORMAL         /* Uncomment this line to enable the audio \
                                         Transfer using DMA */
/* #define AUDIO_MAL_MODE_CIRCULAR */ /* Uncomment this line to enable the audio
                                         Transfer using DMA */

/* For the DMA modes select the interrupt that will be used */
/* #define AUDIO_MAL_DMA_IT_TC_EN */ /* Uncomment this line to enable DMA Transfer Complete interrupt */
/* #define AUDIO_MAL_DMA_IT_HT_EN */ /* Uncomment this line to enable DMA Half Transfer Complete interrupt */
#define AUDIO_MAL_DMA_IT_TE_EN /* Uncomment this line to enable DMA Transfer Error interrupt */

/* #define USE_DMA_PAUSE_FEATURE */ /* Uncomment this line to enable the use of DMA Pause Feature
                                 When this define is enabled, the Pause function will
                                 just pause/resume the DMA without need to reinitialize the
                                 DMA structure. In this case, the buffer pointer should remain
                                 available after resume. */

/* Select the interrupt preemption priority and subpriority for the DMA interrupt */
#define EVAL_AUDIO_IRQ_PREPRIO 0 /* Select the preemption priority level(0 is the highest) */
#define EVAL_AUDIO_IRQ_SUBRIO 0  /* Select the sub-priority level (0 is the highest) */

/* Uncomment the following line to use the default Codec_TIMEOUT_UserCallback() 
   function implemented in stm324xg_evel_audio_codec.c file.
   Codec_TIMEOUT_UserCallback() function is called whenever a timeout condition 
   occurs during communication (waiting on an event that doesn't occur, bus 
   errors, busy devices ...). */
/* #define USE_DEFAULT_TIMEOUT_CALLBACK */
/*----------------------------------------------------------------------------*/

/*------------------------------------
                    OPTIONAL Configuration defines parameters
                                      ----------------------------------------*/
/* I2C clock speed configuration (in Hz) 
  WARNING: 
   Make sure that this define is not already declared in other files (ie. 
  stm324xg_eval.h file). It can be used in parallel by other modules. */
#ifndef I2C_SPEED
#define I2C_SPEED 300000
#endif /* I2C_SPEED */

/* Uncomment defines below to select standard for audio communication between 
  Codec and I2S peripheral */
#define I2S_STANDARD_PHILLIPS
/* #define I2S_STANDARD_MSB */
/* #define I2S_STANDARD_LSB */

/* Uncomment the defines below to select if the Master clock mode should be 
  enabled or not */
//#define CODEC_MCLK_ENABLED
#define CODEC_MCLK_DISABLED

/* Uncomment this line to enable verifying data sent to codec after each write 
  operation */
/* #define VERIFY_WRITTENDATA */
/*----------------------------------------------------------------------------*/

/*-----------------------------------
                    Hardware Configuration defines parameters
                                     -----------------------------------------*/
/* I2S peripheral configuration defines */
#define CODEC_I2S SPI2
#define CODEC_I2S_CLK RCC_APB1Periph_SPI2
#define CODEC_I2S_ADDRESS 0x4000380C
#define CODEC_I2S_GPIO_AF GPIO_AF_SPI2
#define CODEC_I2S_IRQ SPI2_IRQn
#define CODEC_I2S_GPIO_CLOCK (RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOB)
#define CODEC_I2S_WS_PIN GPIO_Pin_12
#define CODEC_I2S_SCK_PIN GPIO_Pin_10
#define CODEC_I2S_SD_PIN GPIO_Pin_15
#define CODEC_I2S_MCK_PIN GPIO_Pin_6
#define CODEC_I2S_WS_PINSRC GPIO_PinSource12
#define CODEC_I2S_SCK_PINSRC GPIO_PinSource10
#define CODEC_I2S_SD_PINSRC GPIO_PinSource15
#define CODEC_I2S_MCK_PINSRC GPIO_PinSource6
#define CODEC_I2S_GPIO GPIOB
#define CODEC_I2S_MCK_GPIO GPIOC

/* I2S DMA Stream definitions */
#define AUDIO_MAL_DMA_CLOCK RCC_AHB1Periph_DMA1
#define AUDIO_MAL_DMA_STREAM DMA1_Stream4
#define AUDIO_MAL_DMA_CHANNEL DMA_Channel_0
#define AUDIO_MAL_DMA_IRQ DMA1_Stream4_IRQn
#define AUDIO_MAL_DMA_FLAG_TC DMA_FLAG_TCIF4
#define AUDIO_MAL_DMA_FLAG_HT DMA_FLAG_HTIF4
#define AUDIO_MAL_DMA_FLAG_FE DMA_FLAG_FEIF4
#define AUDIO_MAL_DMA_FLAG_TE DMA_FLAG_TEIF4
#define AUDIO_MAL_DMA_FLAG_DME DMA_FLAG_DMEIF4
#define AUDIO_MAL_DMA_FLAG_ALL (uint32_t)(AUDIO_MAL_DMA_FLAG_TC | AUDIO_MAL_DMA_FLAG_HT | \
                                          AUDIO_MAL_DMA_FLAG_FE | AUDIO_MAL_DMA_FLAG_DME)
#define AUDIO_MAL_DMA_PERIPH_DATA_SIZE DMA_PeripheralDataSize_HalfWord
#define AUDIO_MAL_DMA_MEM_DATA_SIZE DMA_MemoryDataSize_Word
#define DMA_MAX_SZE 0xFFFF
#define AUDIO_MAL_DMA DMA1
#define AUDIO_MAL_DMA_IFCR HIFCR

#define Audio_MAL_IRQHandler DMA1_Stream4_IRQHandler

/* I2C peripheral configuration defines (control interface of the audio codec) */
#define CODEC_I2C I2C1
#define CODEC_I2C_CLK RCC_APB1Periph_I2C1
#define CODEC_I2C_GPIO_CLOCK RCC_AHB1Periph_GPIOB
#define CODEC_I2C_GPIO_AF GPIO_AF_I2C1
#define CODEC_I2C_GPIO GPIOB
#define CODEC_I2C_SCL_PIN GPIO_Pin_6
#define CODEC_I2C_SDA_PIN GPIO_Pin_7
#define CODEC_I2S_SCL_PINSRC GPIO_PinSource6
#define CODEC_I2S_SDA_PINSRC GPIO_PinSource7

/* Maximum Timeout values for flags and events waiting loops. These timeouts are
   not based on accurate values, they just guarantee that the application will 
   not remain stuck if the I2C communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */
#define CODEC_FLAG_TIMEOUT ((uint32_t)0x1000)
#define CODEC_LONG_TIMEOUT ((uint32_t)(300 * CODEC_FLAG_TIMEOUT))
/*----------------------------------------------------------------------------*/

/*-----------------------------------
                        Audio Codec User defines
                                     -----------------------------------------*/
/* Codec output DEVICE */
#define OUTPUT_DEVICE_SPEAKER 1
#define OUTPUT_DEVICE_HEADPHONE 2
#define OUTPUT_DEVICE_BOTH 3
#define OUTPUT_DEVICE_AUTO 4

/* Volume Levels values */
#define DEFAULT_VOLMIN 0x00
#define DEFAULT_VOLMAX 0xFF
#define DEFAULT_VOLSTEP 0x04

#define AUDIO_PAUSE 0
#define AUDIO_RESUME 1

/* Codec POWER DOWN modes */
#define CODEC_PDWN_HW 1
#define CODEC_PDWN_SW 2

/* MUTE commands */
#define AUDIO_MUTE_ON 1
#define AUDIO_MUTE_OFF 0
/*----------------------------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
#define VOLUME_CONVERT(x) ((Volume > 100) ? 100 : ((uint8_t)((Volume * 255) / 100)))
#define DMA_MAX(x) (((x) <= DMA_MAX_SZE) ? (x) : DMA_MAX_SZE)

/* Exported functions ------------------------------------------------------- */
/*-----------------------------------
                           Generic functions 
                                    ------------------------------------------*/
uint32_t EVAL_AUDIO_Init(uint16_t OutputDevice, uint8_t Volume, uint32_t AudioFreq);
uint32_t EVAL_AUDIO_DeInit(void);
uint32_t EVAL_AUDIO_Play(uint16_t *pBuffer, uint32_t Size);
uint32_t EVAL_AUDIO_PauseResume(uint32_t Cmd, uint32_t Addr, uint32_t Size);
uint32_t EVAL_AUDIO_Stop(uint32_t CodecPowerDown_Mode);
uint32_t EVAL_AUDIO_VolumeCtl(uint8_t Volume);
uint32_t EVAL_AUDIO_Mute(uint32_t Command);
uint32_t Codec_SwitchOutput(uint8_t Output);

/*-----------------------------------
                           Audio Codec functions 
                                    ------------------------------------------*/
/* High Layer codec functions */
uint32_t Codec_Init(uint16_t OutputDevice, uint8_t Volume, uint32_t AudioFreq);
uint32_t Codec_DeInit(void);
uint32_t Codec_Play(void);
uint32_t Codec_PauseResume(uint32_t Cmd);
uint32_t Codec_Stop(uint32_t Cmd);
uint32_t Codec_VolumeCtrl(uint8_t Volume);
uint32_t Codec_Mute(uint32_t Cmd);
void Audio_MAL_IRQHandler(void);

/*-----------------------------------
                   MAL (Media Access Layer) functions 
                                    ------------------------------------------*/
void Audio_MAL_Init(void);
void Audio_MAL_DeInit(void);
void Audio_MAL_Play(uint32_t Addr, uint32_t Size);
void Audio_MAL_PauseResume(uint32_t Cmd, uint32_t Addr, uint32_t Size);
void Audio_MAL_Stop(void);

/* User Callbacks: user has to implement these functions in his code if
  they are needed. -----------------------------------------------------------*/

/* This function is called when the requested data has been completely transferred.
   In Normal mode (when  the define AUDIO_MAL_MODE_NORMAL is enabled) this function
   is called at the end of the whole audio file.
   In circular mode (when  the define AUDIO_MAL_MODE_CIRCULAR is enabled) this 
   function is called at the end of the current buffer transmission. */
void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size);

/* This function is called when half of the requested buffer has been transferred 
   This callback is useful in Circular mode only (when AUDIO_MAL_MODE_CIRCULAR 
   define is enabled)*/
void EVAL_AUDIO_HalfTransfer_CallBack(uint32_t pBuffer, uint32_t Size);

/* This function is called when an Interrupt due to transfer error on or peripheral
   error occurs. */
void EVAL_AUDIO_Error_CallBack(void *pData);

/* Codec_TIMEOUT_UserCallback() function is called whenever a timeout condition 
   occurs during communication (waiting on an event that doesn't occur, bus 
   errors, busy devices ...) on the Codec control interface (I2C).
   You can use the default timeout callback implementation by uncommenting the 
   define USE_DEFAULT_TIMEOUT_CALLBACK in stm324xg_usb_audio_codec.h file.
   Typically the user implementation of this callback should reset I2C peripheral
   and re-initialize communication or in worst case reset all the application. */
uint32_t Codec_TIMEOUT_UserCallback(void);

#endif /* __STM324xG_USB_AUDIOCODEC_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
