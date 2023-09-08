from typing import List
import Pre_processing
import pandas as pd
import pylsl
import time
from typing import List
from threading import Thread
import pylsl
from typing import List
from timeit import default_timer as timer
import pickle
import time
import threading
import numpy as np
from imblearn.over_sampling import SMOTE
from sklearn import svm
from sklearn.metrics import accuracy_score, confusion_matrix
import os
from datetime import datetime
from sklearn.model_selection import KFold


LSL_ET_NAME                 = "Unity.ExampleStream"
LSL_EEG_DATA_NAME           = "EmotivDataStream-EEG"
LSL_EEG_DATA_QUALITY_NAME   = "EmotivDataStream-EEG-Quality"
LSL_RESULT_NAME             = "HMIResultData"

SFREQ = 128.
LOW_PASS = 0.16
HIGH_PASS = 50.
RAW_SAMPLING_RATE: int = 128

BUFFER_TIME = 4  # seconds
#BUFFER_SIZE = BUFFER_TIME * RAW_SAMPLING_RATE
BUFFER_SIZE=16


SEGMENT_SIZE = 64
SEGMENT_OVERLAP = 0

SAMPLING_RATE = 128
INTERPCOLs = ['Cz', 'Fz', 'Fp1', 'F7', 'F3', 'FC1', 'C3', 'FC5', 'FT9', 'T7', 'CP5',
              'CP1', 'P3', 'P7', 'PO9', 'O1', 'Pz', 'Oz', 'O2', 'PO10', 'P8', 'P4', 
              'CP2', 'CP6', 'T8', 'FT10', 'FC6', 'C4', 'FC2', 'F4', 'F8', 'Fp2']
PICKs = ['Fp1', 'Fp2', 'O1', 'O2']
MINIMUM_DATA_COUNT=25

with open('D:/implement_AI_VinIf - Copy/model.pkl','rb') as f:
    model=pickle.load(f)
def predict(eeg_signal):
    predict=model.predict_proba(eeg_signal)
    return predict

class ETReceive():
    """docstring for ETReceive"""

    def __init__(self):
        super(ETReceive, self).__init__()
        self.stt = False
        streams = pylsl.resolve_stream()
        for stream in streams:
            if stream.name() == LSL_ET_NAME:
                self.inlet = pylsl.StreamInlet(stream)
                self.stt = True

        if not hasattr(self, 'inlet'):

            print('Không kết nối được với chương trình thu.')

        self.lastSample = None
        self.lSample: List[float] = []
        
    def update(self):
        timestamps, sample = None, None
        if self.stt:
            sample, timestamps = self.inlet.pull_sample(timeout=1000 / 30)
            return sample[0]
    
class EEGReceive():
    """docstring for ETReceive"""

    def __init__(self):
        super(EEGReceive, self).__init__()
        streams = pylsl.resolve_stream()
        for stream in streams:
            if stream.name() == LSL_EEG_DATA_NAME:
                self.inlet = pylsl.StreamInlet(stream)
            if stream.name() == LSL_EEG_DATA_QUALITY_NAME:
                self.quality_inlet = pylsl.StreamInlet(stream)

        if not hasattr(self, 'inlet'):
            print('Không tìm thấy dữ liệu điện não.')

        self.lData=[]
        self.lTimeStamp=[]

    def getSavingData(self):
        return self.lData

    def update(self):
        while True:
            chunk, timestamps = self.inlet.pull_chunk()
            if len(timestamps) > 0:
                for idx, _ in enumerate(timestamps):
                    self.lData.append(chunk[idx])
            if len(self.lData)>SEGMENT_SIZE:
                while len(self.lData)>SEGMENT_SIZE:
                    self.lData.pop(0)

            # if the new cursor is different from the current cursor, interrupt the current process

def processing(eeg_data, time):
    rows_list_dataset = pd.DataFrame()
    for channel in INTERPCOLs:
        Feature_dataset = Pre_processing.PrepareData(eeg_data, channel)
        Feature_dataset_rs = Pre_processing.MergeFeature(Feature_dataset)
        Feature_dataset_tmp = []
        for row in Feature_dataset_rs:
            tmp = {'alpha_'+channel: row[0], 'beta_'+channel: row[1], 'theta_'+channel: row[2], 'delta_'+channel: row[3]}
            Feature_dataset_tmp.append(tmp)
        tmp_df = pd.DataFrame(Feature_dataset_tmp)
        if rows_list_dataset.empty:
            rows_list_dataset = tmp_df
        else:
            rows_list_dataset = rows_list_dataset.join(tmp_df, how='left')
    df_dataset = pd.DataFrame(rows_list_dataset)
    df_dataset=df_dataset.astype(str)
    for col in df_dataset.columns:
      df_dataset[col] = df_dataset[col].apply(lambda x: x.lstrip('(').rstrip('+0j)'))
    return df_dataset


    
def data_initiate(eeg):
    eeg_rs=[]
    eeg_rs= eeg
    eeg_rs_rs = pd.DataFrame(eeg_rs,columns=['time','1','2','Cz', 'Fz', 'Fp1', 'F7', 'F3', 'FC1', 'C3', 'FC5', 'FT9', 'T7', 'CP5',
            'CP1', 'P3', 'P7', 'PO9', 'O1', 'Pz', 'Oz', 'O2', 'PO10', 'P8', 'P4', 
            'CP2', 'CP6', 'T8', 'FT10', 'FC6', 'C4', 'FC2', 'F4', 'F8', 'Fp2','35','36'])
    eeg_rs_rs=eeg_rs_rs.drop(['1', '2','35','36'], axis=1)
    eeg_rs_rs = eeg_rs_rs.apply(pd.to_numeric)
    eeg_rs_rs=eeg_rs_rs*1000
    return eeg_rs_rs


def checkCrossPerson(pos_data, neg_data):
    with open('D:/implement_AI_VinIf - Copy/model.pkl','rb') as f:
        model=pickle.load(f)

    train_y1 = np.zeros((pos_data.shape[0],))
    train_y1[:] = 1
    train_y2 = np.zeros((neg_data.shape[0],))

    train_y = np.concatenate((train_y1, train_y2))
    train_x = pd.concat([pos_data, neg_data], ignore_index=True)
    train_x = train_x.to_numpy()

    over = SMOTE(k_neighbors=2)
    train_x, train_y = over.fit_resample(train_x, train_y)

    n_splits = 5
    kf = KFold(n_splits=n_splits)
    total_accuracy = 0
    total_training_time = 0

    for train_index, test_index in kf.split(train_x):
        X_train, X_test = train_x[train_index], train_x[test_index]
        y_train, y_test = train_y[train_index], train_y[test_index]

        start_time = time.time()
        model.fit(X_train, y_train)
        training_time = time.time() - start_time
        total_training_time += training_time

        y_pred = model.predict(X_test)
        accuracy = accuracy_score(y_test, y_pred)
        total_accuracy += accuracy

    avg_accuracy = total_accuracy / n_splits

    excel_file = 'accuracy_results.xlsx'
    if not os.path.isfile(excel_file):
        df = pd.DataFrame(columns=['Accuracy', 'Total Training Time'])
        df.to_excel(excel_file, index=False)

    df = pd.read_excel(excel_file)

    df = df.append({'Accuracy': avg_accuracy, 'Total Training Time': total_training_time}, ignore_index=True)

    df.to_excel(excel_file, index=False)
    with open('D:/implement_AI_VinIf - Copy/model.pkl', 'wb') as f:
        pickle.dump(model, f)
class ResultSend():
    def __init__(self):
        self.streamInfo = pylsl.StreamInfo(LSL_RESULT_NAME, 'Markers', 1, 0, 'float32', 'myuidw43536')
        self.outlet = pylsl.StreamOutlet(self.streamInfo)
        self.ET = ETReceive()
        self.EEG = EEGReceive()
        self.EEG_inlet=Thread(target=self.EEG.update)
        self.EEG_inlet.daemon= True
        self.EEG_inlet.start()
        self.lData=self.EEG.getSavingData()

        self.current_cursor = None
        self.start_time = None
        self.result = None
        self.result_sent = False

        self.positive_data = pd.DataFrame()
        self.negative_data = pd.DataFrame()
        self.prev_et_value = None
        self.et_value_count = 0


        # create and start the thread for checking cursor position
        self.cursor_thread = threading.Thread(target=self.check_cursor)
        self.cursor_thread.daemon = True
        self.cursor_thread.start()
            
        # create and start the thread for saving data
        self.data_saving_thread = threading.Thread(target=self.save_data)
        self.data_saving_thread.daemon = True
        self.data_saving_thread.start()

        # create and start the thread for training data
        self.train_thread = threading.Thread(target=self.train_model)
        self.train_thread.daemon = True
        self.train_thread.start()

                
    def check_cursor(self):
        while True:
            new_cursor = self.ET.update()
            if new_cursor != self.current_cursor:
                self.current_cursor = None
                self.start_time = None
                self.result = None

    def send_data(self):
        time.sleep(1)
        while True:
            eegdata = self.EEG.getSavingData()
            if self.current_cursor is None and self.start_time is None and len(eegdata) == 64:
                self.current_cursor = self.ET.update()
                data = data_initiate(eegdata)
                segment_data = processing(data, self.current_cursor)
                self.result = predict(segment_data)
            elif self.result is not None:
                if self.result[0][0] >0.5:
                    self.outlet.push_sample([-self.result[0][0]])
                    #print(-self.result[0][0])
                else:
                    self.outlet.push_sample([self.result[0][1]])
                    #print(self.result[0][1]) 
                self.current_cursor = None
                self.start_time = None
                self.result = None
    def train_model(self):
        time.sleep(1)
        while True:
            if len(self.positive_data) >= 25:
                print("Start trainnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn")
                print(self.positive_data)
                checkCrossPerson(self.positive_data, self.negative_data)
                # Reset self.positive_data và self.negative_data
                self.positive_data = pd.DataFrame()
                self.negative_data = pd.DataFrame()
                print("TRAINING DONEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE")

    #save data
    def save_data(self):
        time.sleep(1)
        last_et_update_time = time.time()

        while True:
            et_value = self.ET.update()
            if self.prev_et_value is None:
                self.prev_et_value = et_value

            # Positive data
            if et_value == self.prev_et_value:
                current_time = time.time()
                if current_time - last_et_update_time >= 1.5:  # 1.5 seconds of same et_value
                    eeg_data = self.EEG.getSavingData()
                    data = data_initiate(eeg_data)
                    segment_data = processing(data, et_value)
                    self.positive_data = self.positive_data.append(segment_data, ignore_index=True)
                    print("Positive~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~")
                    print(len(self.positive_data))
                    last_et_update_time = time.time()

            else:
                # Negative data
                eeg_data = self.EEG.getSavingData()
                data = data_initiate(eeg_data)
                segment_data = processing(data, et_value)
                self.negative_data = self.negative_data.append(segment_data, ignore_index=True)
                print("NEGATIVE APEND-------------------------------------------")
                last_et_update_time = time.time()
            self.prev_et_value = self.ET.update()
    




sender = ResultSend()
sender.send_data()
