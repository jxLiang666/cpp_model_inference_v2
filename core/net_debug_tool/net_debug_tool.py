import numpy as np
import os


class NetDebugTool(object):
    @staticmethod
    def save_numpy_to_binary_file(data: np.ndarray, save_dir: str):
        if not isinstance(data, np.ndarray):
            raise TypeError("Data must be type of numpy.ndarray!")
        if not isinstance(save_dir, str):
            raise TypeError("Save_dir must be type of str!")
        data_type = data.dtype
        data = data.reshape(-1)  # 保证内存上连续

        print(f"{'Data type:'} {f'{data_type}'}")
        print(f"{'Element size:'} {f'{data.itemsize} Byte'}")
        print(f"{'Element count:'} {f'{len(data)} 个'}")
        print(f"{'Total size:'} {f'{data.itemsize*len(data)} Byte'}")
        print(f"Save dir: {save_dir}")

        os.makedirs(os.path.dirname(save_dir), exist_ok=True)
        data.tofile(save_dir)

    @staticmethod
    def save_numpy_to_txt_file(data: np.ndarray, save_dir: str):
        if not isinstance(data, np.ndarray):
            raise TypeError("Data must be type of numpy.ndarray!")
        if not isinstance(save_dir, str):
            raise TypeError("Save_dir must be type of str!")

        data_type = data.dtype
        if np.issubdtype(data_type, np.integer):
            fmt = "%d"  # 整数格式
        elif np.issubdtype(data_type, np.floating):
            fmt = "%.6f"  # 浮点格式，6位小数
        else:
            raise TypeError(f"Unsupported data type: {data_type}")

        print(f"Data type: {data_type}, save type: {fmt}")
        print(f"{'Element size:'} {data.itemsize} Byte")
        print(f"{'Total size:'} {data.itemsize * data.size} Byte")
        print(f"Save dir: {save_dir}")

        os.makedirs(os.path.dirname(save_dir), exist_ok=True)
        np.savetxt(save_dir, data, fmt=fmt)

    @staticmethod
    def read_numpy_from_binary_file(file_path: str, dtype) -> np.ndarray:
        if not isinstance(file_path, str):
            raise TypeError("file_path must be type of str")
        dtype = np.dtype(dtype)
        print(f"Read path: {file_path}")
        print(f"Read data type: ", dtype)
        data = np.fromfile(f"{file_path}", dtype=dtype)
        # 获取文件大小（字节）
        file_size = os.path.getsize(file_path)
        print(f"File size: {file_size} Byte")
        data_size = data.itemsize * len(data)
        print(f"{'Total size:'} {f'{data_size} Byte'}")
        assert (
            data_size == file_size
        ), "Total size is not equal to file size, please check the data type right or not, p.s. np.float32, np.uint8"
        return data

    @staticmethod
    def analyze_files(cpp_data_file_path: str, py_data_file_path: str, dtype):
        out_py = NetDebugTool.read_numpy_from_binary_file(
            py_data_file_path,
            dtype,
        )
        out_cpp = NetDebugTool.read_numpy_from_binary_file(
            cpp_data_file_path,
            dtype,
        )

        assert len(out_py) == len(
            out_cpp
        ), f"The data lenths of [{cpp_data_file_path},{py_data_file_path}] are not same, please check!！"

        size = len(out_cpp)

        for i in range(size):
            if not (out_cpp[i] == out_py[i]):
                {
                    print(
                        f"The data is not same at idx of {i}\n"
                        f"In {cpp_data_file_path}, it is {out_cpp[i]}\n"
                        f"In {py_data_file_path}, it is {out_py[i]}"
                    )
                }

        print(
            f"[{cpp_data_file_path.split('/')[-1]}] and [{py_data_file_path.split('/')[-1]}], the all data is same! Congrat!"
        )
