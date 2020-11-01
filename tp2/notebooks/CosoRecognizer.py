import tkinter as tk
import pandas as pd
import numpy as np
from PIL import Image
import io
import os
import metnum


def draw(event):
    x, y = event.x, event.y
    if canvas.old_coords:
        x1, y1 = canvas.old_coords
        canvas.create_oval(x,y,x1,y1,
            fill='black',
            width=25)
    canvas.old_coords = x, y

def reset_coords(event):
    canvas.old_coords = None

def reconocer():
    ps = canvas.postscript(colormode='mono')
    img = Image.open(io.BytesIO(ps.encode('utf-8'))).resize((28,28)).convert(mode='L')
    X_val = 255 - (np.array(img).reshape(1,784))
    # img.save('filename.jpg', 'jpeg')
    X_val_pca = pca.transform(X_val)
    clf.load(X_val_pca)
    y_pred = clf.predict(k)

    resultadoStr.set(f"Se supone que es un {int(y_pred[0])}")

    # df_x = pd.DataFrame(data=X_val.reshape(28,28))
    # df_x.to_csv(f"img.csv",index=False)

def clean():
    canvas.delete("all")
    resultadoStr.set("Dibujate otro numeracho")

if __name__ == '__main__':
# ---------------------
    alpha = 35
    k = 3

    df_train = pd.read_csv("../data/train.csv")
    X = df_train[df_train.columns[1:]].values
    y = df_train["label"].values.reshape(-1, 1)
    
    pca= metnum.PCA(alpha)

    try:
        pca_tc = pd.read_csv('PCA_TC_coso_recog.csv').to_numpy()
        pca.setTC(pca_tc)
        print('loaded tc from csv')
    except:  
        print('calculating tc')
        pca.fit(X)
        pd.DataFrame(data=pca.getTC()).to_csv('PCA_TC_coso_recog.csv', index=False)
    
    X_pca = pca.transform(X)

    clf = metnum.KNNClassifier(k)
    clf.fit(X_pca, y)
# --------------------------------
    root = tk.Tk()
    root.title('Coso Recognizer')

    canvas = tk.Canvas(root, width=280, height=280, bg='white')
    canvas.pack()
    canvas.old_coords = None

    root.bind('<B1-Motion>', draw)
    root.bind('<ButtonRelease-1>', reset_coords)

    botonera = tk.Frame(root)
    botonera.pack()
    button = tk.Button(botonera, text='Reconocer', command=reconocer)
    button.pack(side=tk.LEFT)

    button = tk.Button(botonera, text='Borrar', command=clean)
    button.pack(side=tk.LEFT)

    resultadoStr = tk.StringVar()
    resultadoStr.set("Dibujate un numeracho")
    resultado = tk.Label(root, textvariable=resultadoStr)
    resultado.pack()

    root.mainloop()