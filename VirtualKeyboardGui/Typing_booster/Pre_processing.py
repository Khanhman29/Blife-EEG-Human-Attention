import numpy as np
import pandas as pd
from scipy import stats


SFREQ = 128.
LOW_PASS = 0.16
HIGH_PASS = 50.

SEGMENT_SIZE = 64
SEGMENT_OVERLAP = 0

SAMPLING_RATE = 128
INTERPCOLs = ['Cz', 'Fz', 'Fp1', 'F7', 'F3', 'FC1', 'C3', 'FC5', 'FT9', 'T7', 'CP5',
              'CP1', 'P3', 'P7', 'PO9', 'O1', 'Pz', 'Oz', 'O2', 'PO10', 'P8', 'P4', 
              'CP2', 'CP6', 'T8', 'FT10', 'FC6', 'C4', 'FC2', 'F4', 'F8', 'Fp2']
PICKs = ['Fp1', 'Fp2', 'O1', 'O2']

def Merge(eeg_raw, et, ts):
  # We will make a vlookup TS in ET dataframe with range searching
  # https://stackoverflow.com/questions/40590998/lookup-value-in-dictionary-between-range-pandas
  max_temp = []
  max_temp.append(0)
  for i in range(0, len(et['TimeStamp'])-1):
    max_temp.append(et['TimeStamp'][i])
  et['TimeStart'] = max_temp
  ts['Time'] = pd.cut(ts.timestamp, bins=[0] + et[['TimeStart','TimeStamp']].astype(str).stack()[1::2].tolist(), labels=et.TimeStamp.tolist(), ordered=False)
  #eeg_df = eeg_raw.to_data_frame()
  eeg_df = eeg_raw.join(ts, how='left')
  return eeg_df

def Norm(data):
  mean = np.mean(data)
  rs = data - mean
  rs = stats.zscore(rs, axis=None)

  return rs
def SplitChunk(data):
  result=[]
  x=Norm(data)
  result.append(x)
  return result

def GetFFT(datas):
  #sampling_length = SEGMENT_SIZE
  fft_rs = []

  for data in datas:
    ts = 1.0/SAMPLING_RATE
    freq = np.fft.fftfreq(len(data), d = ts)
    fft = np.fft.fft(data)
    fft_rs.append(fft)

  return fft_rs, freq

# Calculate Power Spectral Density(PSD) (equation 1)
def GetPSD(datas):
  #sampling_length = SEGMENT_SIZE
  PSD_rs = []
  for data in datas:
    PSD_seg = []
    for d in data:
      PSD_n = d * np.conjugate(d)/SEGMENT_SIZE
      PSD_seg.append(PSD_n)
    PSD_rs.append(PSD_seg)
  
  return PSD_rs

def GetSegmentsFeature(datas, freqs):
  # Equation 2, 3, 4, 5
  feature_rs = []

  for data in datas:
    d = {'data': data, 'freq':freqs}
    fft_dt = pd.DataFrame(data = d)

    feature = []
    # Equation 2: 8 -> 13
    alpha_f = fft_dt[fft_dt['freq']>=8]
    alpha_f = alpha_f[alpha_f['freq']<=13]
    alpha = np.sum(alpha_f['data'])

    # Equation 3: 14 -> 30
    beta_f = fft_dt[fft_dt['freq']>=14]
    beta_f = beta_f[beta_f['freq']<=30]
    beta = np.sum(beta_f['data'])

    # Equation 4: 4 -> 7
    theta_f = fft_dt[fft_dt['freq']>=4]
    theta_f = theta_f[theta_f['freq']<=7]
    theta = np.sum(theta_f['data'])

    # Equation 5: 0.5 -> 3
    delta_f = fft_dt[fft_dt['freq']>=0.5]
    delta_f = delta_f[delta_f['freq']<=3]
    delta = np.sum(delta_f['data'])

    feature.append(alpha)
    feature.append(beta)
    feature.append(theta)
    feature.append(delta)
    feature_rs.append(feature)
  return feature_rs

#def PrepareData(eeg_raw, eeg_ts, et_df, channel):
def PrepareData(eeg_raw, channel):

  Feature_Dataset_rs = []
  # labeling data
  #merge = Merge(eeg_raw, et_df, eeg_ts)

  # Split data to datasetitive and negative
  eeg_df = eeg_raw
  #data= SplitData(eeg_df[channel]) 
  data=eeg_df[channel]

  data=data.to_numpy()
  
  # Splip datasetitive and negative data to segments
  Dataset_Segments = SplitChunk(data)

  # switch segments to FFT domain
  fft_Dataset, freq_Dataset = GetFFT(Dataset_Segments)


  # calculate PSD value of segments
  PSD_Dataset = GetPSD(fft_Dataset)
    
  # calculate feature vector of segments
  Feature_Dataset = GetSegmentsFeature(PSD_Dataset, freq_Dataset)
  Feature_Dataset_rs.append(Feature_Dataset)
  return Feature_Dataset_rs

def MergeFeature(Feature_Dataset):
  Feature_Dataset_rs = []
  for dataset in Feature_Dataset:
    for p in dataset:
      Feature_Dataset_rs.append(p)
  return Feature_Dataset_rs

  


