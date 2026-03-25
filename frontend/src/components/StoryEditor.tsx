import React, { useState } from 'react';
import { Sparkles, FileText, Users, MapPin, Zap } from 'lucide-react';

interface Character {
  id: string;
  name: string;
  description: string;
  personality: string;
}

interface Scene {
  id: string;
  name: string;
  location: string;
  description: string;
}

const StoryEditor: React.FC = () => {
  const [storyText, setStoryText] = useState<string>(`在一个遥远的星球上，年轻的探险家艾莉发现了一座神秘的古老遗迹。
她的同伴马克提醒她要小心，但艾莉的好奇心驱使她继续前进。
当他们进入遗迹深处时，遇到了守护此地的神秘机器人...`);

  const [characters, setCharacters] = useState<Character[]>([
    { id: '1', name: '艾莉', description: '年轻的探险家', personality: '好奇心强, 勇敢无畏' },
    { id: '2', name: '马克', description: '谨慎的同伴', personality: '细心, 稳健' },
  ]);

  const [scenes, setScenes] = useState<Scene[]>([
    { id: '1', name: '遗迹入口', location: '遥远星球', description: '神秘的古老遗迹入口' },
  ]);

  const [isAnalyzing, setIsAnalyzing] = useState(false);

  const handleAnalyze = () => {
    setIsAnalyzing(true);
    setTimeout(() => {
      setIsAnalyzing(false);
    }, 1500);
  };

  return (
    <div className="flex flex-col h-full">
      <div className="h-12 border-b border-slate-700 flex items-center justify-between px-4 bg-slate-800/50">
        <h2 className="text-lg font-semibold text-slate-200">叙事编辑器</h2>
        <div className="flex items-center gap-2">
          <button
            onClick={handleAnalyze}
            disabled={isAnalyzing}
            className="flex items-center gap-2 px-4 py-1.5 bg-purple-600 hover:bg-purple-700 disabled:bg-slate-600 rounded-md text-sm font-medium transition-colors"
          >
            {isAnalyzing ? (
              <div className="w-4 h-4 border-2 border-white/30 border-t-white rounded-full animate-spin" />
            ) : (
              <Sparkles className="w-4 h-4" />
            )}
            {isAnalyzing ? '分析中...' : 'AI 分析'}
          </button>
        </div>
      </div>

      <div className="flex flex-1 overflow-hidden">
        <div className="flex-1 flex flex-col border-r border-slate-700">
          <div className="flex-1 p-4 overflow-hidden flex flex-col">
            <label className="text-sm text-slate-400 mb-2">故事文本</label>
            <textarea
              value={storyText}
              onChange={(e) => setStoryText(e.target.value)}
              className="flex-1 w-full bg-slate-800 border border-slate-600 rounded-lg p-4 text-slate-200 resize-none focus:outline-none focus:ring-1 focus:ring-purple-500 focus:border-transparent font-mono text-sm leading-relaxed"
              placeholder="在此输入您的故事文本..."
            />
          </div>
        </div>

        <div className="w-80 flex flex-col bg-slate-800/30">
          <div className="flex-1 overflow-y-auto">
            <div className="p-4 border-b border-slate-700">
              <div className="flex items-center gap-2 mb-3">
                <Users className="w-4 h-4 text-blue-400" />
                <h3 className="font-semibold text-slate-200">角色</h3>
              </div>
              <div className="space-y-2">
                {characters.map((char) => (
                  <div key={char.id} className="p-3 bg-slate-700/50 rounded-lg border border-slate-600">
                    <div className="font-medium text-slate-200">{char.name}</div>
                    <div className="text-xs text-slate-400 mt-1">{char.description}</div>
                    <div className="text-xs text-purple-400 mt-1">{char.personality}</div>
                  </div>
                ))}
              </div>
            </div>

            <div className="p-4 border-b border-slate-700">
              <div className="flex items-center gap-2 mb-3">
                <MapPin className="w-4 h-4 text-orange-400" />
                <h3 className="font-semibold text-slate-200">场景</h3>
              </div>
              <div className="space-y-2">
                {scenes.map((scene) => (
                  <div key={scene.id} className="p-3 bg-slate-700/50 rounded-lg border border-slate-600">
                    <div className="font-medium text-slate-200">{scene.name}</div>
                    <div className="text-xs text-slate-400 mt-1">{scene.location}</div>
                    <div className="text-xs text-orange-400 mt-1">{scene.description}</div>
                  </div>
                ))}
              </div>
            </div>

            <div className="p-4">
              <div className="flex items-center gap-2 mb-3">
                <Zap className="w-4 h-4 text-yellow-400" />
                <h3 className="font-semibold text-slate-200">分析结果</h3>
              </div>
              <div className="p-3 bg-slate-700/50 rounded-lg border border-slate-600">
                <div className="text-sm text-slate-300">
                  <div className="mb-2">
                    <span className="text-slate-400">类型：</span>
                    <span className="text-blue-400 ml-2">科幻冒险</span>
                  </div>
                  <div className="mb-2">
                    <span className="text-slate-400">主题：</span>
                    <span className="text-purple-400 ml-2">探索, 勇气</span>
                  </div>
                  <div>
                    <span className="text-slate-400">基调：</span>
                    <span className="text-orange-400 ml-2">神秘, 紧张</span>
                  </div>
                </div>
              </div>
            </div>
          </div>
        </div>
      </div>
    </div>
  );
};

export default StoryEditor;
